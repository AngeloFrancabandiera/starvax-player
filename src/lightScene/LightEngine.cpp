#include "LightEngine.h"
#include "LightPresetModel.h"
#include "LightPresetData.h"
#include "IF_lighttransitionengine.h"
#include "DmxInterface.h"
#include "modelViewRules.h"
#include "ActionListController.h"
#include "testableAssert.h"
#include "StatusDisplay.h"

LightEngine::LightEngine( LightPresetModel & model, IF_LightTransitionEngine & transitionEngine,
                          ActionListController & viewController, DmxInterface & dmxInterface,
                          StatusDisplay & msgDisplay, QObject *parent) :
   IF_LightEngineInterface(parent),
   m_model(model),
   m_transitionEngine(transitionEngine),
   m_viewController(viewController),
   m_dmxInterface( dmxInterface),
   m_msgDisplay( msgDisplay)
{
   connect( & m_transitionEngine, SIGNAL(fadeStep(QList<double>)),
            this, SLOT(onTransitionStep(QList<double>)) );
}

/**
 * @brief activate the trasnsition described by preset labeled as \p label.
 *    NOTE: The preset should be valid, otherwise an exception is rised.
 * @param label - name of preset
 */
void LightEngine::activateByName( const QString &label, bool triggerNow)
{
   QModelIndex index = m_model.searchByName( label);

   if (index != QModelIndex())
   {
      const LightPresetData & preset =
            m_model.data( index, modelViewRules::Binary).value<LightPresetData>();

      m_viewController.activateRequest( index);

      if (triggerNow == true)
      {
         m_transitionEngine.startTransition( preset.levelList(), preset.fadeTime() );
      }
   }
   else
   {
      m_msgDisplay.showMessage( QString("No preset has label: <b>%1</b>").arg(label),
                                StatusDisplay::WARNING);
   }
}

void LightEngine::triggerTransition(const QString &label)
{
   activateByName( label, true);
}

void LightEngine::freezeTransition()
{
   m_transitionEngine.stopTransition();
}

/**
 * @brief interface function to change a value for a channel from
 *   outside light engine module (eg. from GUI)
 * @param level - new intensity for light (0% to 100%)
 * @param channel - DMX channel. If invalid, nothing is done
 */
void LightEngine::setLevelForChannel(double level, int channel)
{
   try
   {
      m_transitionEngine.setLevelForChannel( level, channel);
      m_dmxInterface.setValueForChannel( level, channel);
   }
   catch( QString &)
   {
      /* invalid channel; just do nothing */
   }
}

/**
 * @brief to be called to request to lower DMX interface to start
 *    communication with the DMX device. If communication succedes,
 *    a message will be emitted.
 */
void LightEngine::requestDmxStart()
{
   DmxUsb::State usbState = m_dmxInterface.startDmxService( m_model.numberOfChannels());

   if (usbState == DmxUsb::USB_OK)
   {
      emit dmxStarted();
      emit dmxStartChanged(true);
   }
   else if (usbState == DmxUsb::USB_DEVICE_NOT_OPEN)
   {
      m_msgDisplay.showMessage( tr("DMX device found but no access allowed. Check USB rules."),
                                StatusDisplay::WARNING);
   }
   else
   {
      m_msgDisplay.showMessage( tr("Unable to load DMX device. Check HW or DLL status."),
                                StatusDisplay::WARNING);
   }
}

/**
 * @brief to be called to request to stop DMX interface.
 * Before shutting down, all levels are set to zero, so that a HHTP
 * (Highest Has The Precedence)  mixer can be used by another interafce.
 */
void LightEngine::requestDmxStop()
{
   m_dmxInterface.turnAllChannelsOff();
   m_dmxInterface.stopDmxService();
   emit dmxStopped();
   emit dmxStartChanged(false);
}

/**
 * @brief this slot is connected to a change form transition engine.
 *    New data must be applied to DMX interface and notified to the outside.
 * @param newValues
 *
 * @note this function would be useless if 'dmxInterface::setValueForAllChannels'
 *       could be used as a regular slot
 */
void LightEngine::onTransitionStep(const QList<double> &newValues)
{
   m_dmxInterface.setValueForAllChannels( newValues);

   emit lightsetChanged(newValues);
}

