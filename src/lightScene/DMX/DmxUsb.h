#ifndef DMXUSB_H
#define DMXUSB_H

#include <QObject>
#include <QTimer>

class DmxUsb : public QObject
{
   Q_OBJECT
public:
   DmxUsb( QObject * parent = nullptr);

   enum State
   {
      USB_OK,
      USB_DEVICE_NOT_FOUND,
      USB_DEVICE_NOT_OPEN
   };

   // number of channels supported by protocol DMX 512
   static const int MAX_SUPPORTED_CHANNELS = 512;

   State startDmxService(int numberOfChannels);

   void stopDmxService();

   /**
    * @brief set the value for a channel
    * @param value is in range 0.0 (dark) to 1.0 (full light)
    * @param channel is logical channel (range 0 to \a m_numOfChannels-1)
    */
   void setValueForChannel(double value, int channel);

   /**
    * @brief set value for all channels
    * @param values is a list of values for consequtive channels from 1 on
    */
   void setValueForAllChannels(QList<double> values);

   /** set all channels to dark */
   void turnAllChannelsOff();

private:
   bool m_libraryFound; ///< flag for USB library present in system
   bool m_deviceFound;
   int m_numOfChannels;
   QTimer m_timer;

   quint8 m_levels[MAX_SUPPORTED_CHANNELS];

   /* set to false when sending value for first channel to device.
    * Set to true when a value changes.
    */
   bool m_repeatUpdateFlag;

   /* index (0 based) of 'm_levels' to send to device in next USB packet */
   int m_nextChannelToSend;

private:
   State setupUsbDevice();
   void startUpdating();
   void stopUpdating();
   void sendFirstBlock();
   void sendNextBlock();
   void checkForUpdateComplete();

private slots:
   void onTimerTick();
};

#endif // DMXUSB_H
