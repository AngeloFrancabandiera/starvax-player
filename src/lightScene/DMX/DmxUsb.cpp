#include "DmxUsb.h"
#include "libusb-1.0/libusb.h"
#include "testableAssert.h"

#include <QFile>
#include <QTextStream>

#define DMX_MAX_LEVEL               255.

/** convert a value from percent (double) to dmx level (int) */
#define PERCENT_TO_LEVEL(pc)     (static_cast<quint8>((pc*DMX_MAX_LEVEL/100.)))


namespace  {
const uint16_t VENDOR_ID_VELLEMAN = 0x10cf;
const uint16_t PRODUCT_ID_K8062 = 0x8062;

// usb parameters
const int USB_ENDPOINT = 1;
const int USB_BLOCK_SIZE = 8;
const int USB_TIMEOUT_ms = 25;

// Vellman protocol
const quint8 SEND_FIRST_BLOCK_OPCODE = 4u;   /* send data from channel 1 */
const quint8 SEND_NEXT_BLOCK_OPCODE = 2u;   /* send following block of 7 channels */
const quint8 SEND_SINGLE_DATA_OPCODE = 3u;   /* not used */


/* LOW SPEED HID can accept 8 bytes every 10 ms, but we keep a margin */
const int LOW_SPEED_HID_TIMING_MS = 12;

libusb_device * K8062_device = nullptr;
struct libusb_device_handle* K8062_handle = nullptr;

/** @return value if in range 0% .. 100%,
  * otherwise return the value clipped
  */
double constrainedValue(double value)
{
   double constrained  = value;

   if (constrained < 0.0)
   {
      constrained = 0.0;
   }
   else if (constrained > 100.0)
   {
      constrained = 100.0;
   }

   return constrained;
}

/** low level function that sends 8 byte packet to device */
void write_command ( unsigned char *data )
{
   int actual_length = 0;

   libusb_interrupt_transfer( K8062_handle,
                              USB_ENDPOINT, data, USB_BLOCK_SIZE,
                              & actual_length, USB_TIMEOUT_ms);
}

}  // namespace

DmxUsb::DmxUsb(QObject *parent) :
   QObject (parent),
   m_libraryFound( false),
   m_deviceFound( false),
   m_numOfChannels(0),
   m_repeatUpdateFlag(false),
   m_nextChannelToSend(0)
{
   int res;
   res = libusb_init(nullptr);

   if (res == 0)
   {
      m_libraryFound = true;
      m_timer.setTimerType(Qt::PreciseTimer);
      m_timer.setInterval( LOW_SPEED_HID_TIMING_MS);
      connect( & m_timer, SIGNAL(timeout()), this, SLOT(onTimerTick()));
   }
}


DmxUsb::State DmxUsb::startDmxService(int numberOfChannels)
{
   State res = USB_DEVICE_NOT_FOUND;

   m_numOfChannels = numberOfChannels;
   K8062_device = nullptr; // preset
   m_deviceFound = false;

   if (m_libraryFound)
   {
      res = setupUsbDevice();
      m_deviceFound = (res == USB_OK);
   }

   return  res;
}


DmxUsb::State DmxUsb::setupUsbDevice()
{
   libusb_device **devs = nullptr;
   ssize_t numOfItems = 0;
   State usbRes = USB_DEVICE_NOT_FOUND;

   numOfItems = libusb_get_device_list( nullptr, &devs);
   if (numOfItems >= 0)
   {
      libusb_device *dev;
      int i=0;

      while ( ((dev = devs[i++]) != nullptr) && (K8062_device == nullptr))
      {
         struct libusb_device_descriptor desc;
         int res = libusb_get_device_descriptor(dev, &desc);
         T_ASSERT( res >= 0);

         if ((desc.idVendor == VENDOR_ID_VELLEMAN) && (desc.idProduct == PRODUCT_ID_K8062))
         {
            K8062_device = dev;

            K8062_handle = libusb_open_device_with_vid_pid( nullptr, VENDOR_ID_VELLEMAN, PRODUCT_ID_K8062);
            if( K8062_handle)
            {
               if (libusb_kernel_driver_active(K8062_handle, 0))
               {
                  /*res =*/ libusb_detach_kernel_driver(K8062_handle, 0);
                  // on some OS this function returns error
                  //T_ASSERT (res == 0);
               }

               libusb_claim_interface(K8062_handle, 0);
               usbRes = USB_OK;
            }
            else
            {
               usbRes = USB_DEVICE_NOT_OPEN;
            }
         }
      }
   }

   libusb_free_device_list(devs, 1);  /* free for 1 get */

   return usbRes;
}

void DmxUsb::startUpdating()
{
   if (m_timer.isActive() == false)
   {
      m_timer.start();
   }
}

void DmxUsb::stopUpdating()
{
   m_timer.stop();
}


void DmxUsb::stopDmxService()
{
   if (m_deviceFound )
   {
      libusb_release_interface( K8062_handle, 0);
      libusb_close( K8062_handle);

      K8062_handle = nullptr;
   }

   m_timer.stop();
}

void DmxUsb::setValueForChannel(double value, int channel)
{
   T_ASSERT( channel >= 0);
   T_ASSERT( channel < MAX_SUPPORTED_CHANNELS);

   /* convert 0.0-1.0 to 0-255 */
   m_levels[channel] = PERCENT_TO_LEVEL(constrainedValue(value));

   m_repeatUpdateFlag = true;

   startUpdating();
}

void DmxUsb::setValueForAllChannels(QList<double> values)
{
   for (int i=0; i < values.length(); i++ )
   {
      setValueForChannel( values.at(i), i);
   }
}

void DmxUsb::turnAllChannelsOff()
{
   memset( m_levels, 0, sizeof(m_levels));
   startUpdating();
}

void DmxUsb::onTimerTick()
{
   if (K8062_handle)
   {
      if (m_nextChannelToSend == 0)
      {
         sendFirstBlock();
      }
      else
      {
         sendNextBlock();
         checkForUpdateComplete();
      }
   }
}

/* send channels 1 to 6 */
void DmxUsb::sendFirstBlock()
{
   static unsigned char block[8];

   m_repeatUpdateFlag = false;

   block[0] = SEND_FIRST_BLOCK_OPCODE;
   block[1] = 1;  /* first channel that may be non zero */

   for (int i=2; i <= 7; i++)
   {
      block[i] = m_levels[i-2];
   }

   write_command ( block);

   m_nextChannelToSend = 6;
}

/* send a block of 7  channels */
void DmxUsb::sendNextBlock()
{
   static unsigned char block[8];

   block[0] = SEND_NEXT_BLOCK_OPCODE;

   for (int i=1; i <= 7; i++)
   {
      block[i] = m_levels[m_nextChannelToSend + i - 1];
   }

   write_command ( block);

   m_nextChannelToSend += 7;
}


void DmxUsb::checkForUpdateComplete()
{
   if (m_nextChannelToSend >= m_numOfChannels)
   {
      m_nextChannelToSend = 0;

      /* all channels sent to device. Check if some channel has changed during
       * the update phase */
      if (m_repeatUpdateFlag == false)
      {
         /* all done. */
         m_timer.stop();
      }
   }
}

