#ifndef DMXINTERFACE_K8062_H
#define DMXINTERFACE_K8062_H

#include "DmxInterface.h"


/**
 * @brief The DmxInterface_K8062 class is a concrete implementation
 * of DMX interface for Velleman K8062 device. The implementation depends
 * on a third party library (libusb-1.0)
 */
class DmxInterface_K8062 : public DmxInterface
{
public:
   DmxInterface_K8062();
   ~DmxInterface_K8062() override {}

   // DmxInterface interface
public:
   DmxUsb::State startDmxService(int numberOfChannels) override;

   void stopDmxService() override;

   bool isDmxServiceStarted() override {
      return m_serviceStarted;
   }

   /**
    * @brief
    * @param value is in range 0.0-100.0
    * @param channel starts from 0.
    */
   void setValueForChannel(double value, int channel) override;

   void setValueForAllChannels(QList<double> values) override;

   void turnAllChannelsOff() override;

private:
   bool m_serviceStarted;
   int m_numOfChannels;
   DmxUsb * m_dmxUsb;
};



#endif // DMXINTERFACE_K8062_H
