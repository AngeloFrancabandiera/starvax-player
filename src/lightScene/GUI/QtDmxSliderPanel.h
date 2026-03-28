#ifndef QTDMXSLIDERPANEL_H
#define QTDMXSLIDERPANEL_H

#include "IF_DmxSliderPanel.h"
#include <QWidget>
#include <QList>

class LightChannelWidget;
class IF_LightTransitionEngine;

class QtDmxSliderPanel : public IF_DmxSliderPanel
{
   Q_OBJECT
public:
   explicit QtDmxSliderPanel( int numOfChannels, IF_LightTransitionEngine *transitionEngine,
                              QWidget *parent);

   const QList<double> & values() override;

   /** redefinition of ::IF_DmxSliderPanel::setAllValues */
   void setAllValues( const QList<double> &values) override;

   /** redefinition of ::IF_DmxSliderPanel::setValueAtChannel */
   void setValueAtChannel( double value, int channel) override;

   /** redefinition of ::IF_DmxSliderPanel::setDmxOn */
   void setDmxOn() override;

   /** redefinition of ::IF_DmxSliderPanel::setDmxOff */
   void setDmxOff() override;

   /** redefinition of ::IF_DmxSliderPanel::setManualOnlyChannel
    * @par channelIndex is zero based
    */
   void setManualOnlyChannel(bool manual, int channelIndex) override;

   /**
    * @brief to be called when GUI requests to create preset.
    *    Requests to create a preset with current values
    */
   void onRequestToCreatePreset() override;

private:
   QList<LightChannelWidget *> m_sliderSet;
   QList<double> m_values;

   /** @brief  set to TRUE during update from core notification.
    * when the core updates the value of a channel, the GUI must not
    * notify such change back to avoid a loop.
    */
   bool m_updateInProgress;

private slots:
   void onSliderValueChanged(int value, int channel);
   void onManualOnlyChangedByGUI( bool isManual, int channel);
};

#endif // QTDMXSLIDERPANEL_H
