#ifndef DMXINTERFACE_H
#define DMXINTERFACE_H

#include <QList>
#include "DmxUsb.h"

/**
 * @brief The DmxInterface class is the abstract interafce with HW DMX device.
 */
class DmxInterface
{
public:
   virtual ~DmxInterface(){}

   /**
    * @brief open a connection with DMX device
    * @param numberOfChannels is the number of DMX channels that will be used
    * @return false if an error has been detected. If true is returned, it may mean
    *   that all is fine or that the procedure was not able to detect any error.
    */
   virtual DmxUsb::State startDmxService(int numberOfChannels) = 0;

   /**
    * @brief stop the DMX service. Successive calls to SetValueXXX functions will
    *   not modify light settings. Current light status is preserved: no 'black-out'
    *   command is issued.
    */
   virtual void stopDmxService() = 0;

   /**
    * @return true if DMX service has been activated
    */
   virtual bool isDmxServiceStarted() = 0;

   /**
    * @brief set a value for a DMX channel
    * @param value should in range 0.0 - 100.0. If this is not the case,
    *   behaviour is un-specified.
    * @param channel should be from 0 and the value passed to \a startDmxService
    *   method. If this is not the case, nothing is done.
    */
   virtual void setValueForChannel( double value, int channel) = 0;

   /**
    * @brief apply a list of values to DMX channels
    * @param values should have size equal to the value passed to \a startDmxService
    *   method. It size of \p values is samller, only channels from 1 to this size are set.
    *   If size of \p values is bigger, an error is generated.
    */
   virtual void setValueForAllChannels( QList<double> values) = 0;

   /**
    * @brief set value 0.0 to all managed channels
    */
   virtual void turnAllChannelsOff() = 0;
};

#endif // DMXINTERFACE_H
