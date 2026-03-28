#include "DmxInterface_K8062.h"

#include <QMessageBox>



DmxInterface_K8062::DmxInterface_K8062():
   m_serviceStarted(false),
   m_numOfChannels(0),
   m_dmxUsb( new DmxUsb())
{
}

DmxUsb::State DmxInterface_K8062::startDmxService(int numberOfChannels)
{
   DmxUsb::State state = m_dmxUsb->startDmxService( numberOfChannels);

   if (state == DmxUsb::USB_OK)
   {
      m_serviceStarted = true;
      m_numOfChannels = numberOfChannels;
   }
   else
   {
      m_dmxUsb->stopDmxService();
      m_serviceStarted = false;
   }

   return state;
}


void DmxInterface_K8062::stopDmxService()
{
   if (m_serviceStarted)
   {
      m_dmxUsb->stopDmxService();
      m_serviceStarted = false;
   }
}


void DmxInterface_K8062::setValueForChannel(double value, int channel)
{
   if ((m_serviceStarted) && (channel < m_numOfChannels))
   {
      m_dmxUsb->setValueForChannel( value, channel);
   }
}

void DmxInterface_K8062::setValueForAllChannels(QList<double> values)
{
   if (m_serviceStarted)
   {
      m_dmxUsb->setValueForAllChannels( values);
   }
}

void DmxInterface_K8062::turnAllChannelsOff()
{
   if (m_serviceStarted)
   {
      m_dmxUsb->turnAllChannelsOff();
   }
}

