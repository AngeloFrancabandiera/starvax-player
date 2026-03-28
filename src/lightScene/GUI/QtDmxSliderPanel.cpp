#include <QSlider>
#include <QHBoxLayout>

#include "testableAssert.h"
#include "QtDmxSliderPanel.h"
#include "swiftslider.h"
#include "IF_lighttransitionengine.h"
#include "testableAssert.h"


#include "LightChannelWidget.h"


/** @class QtDmxSliderPanel
 *
 * This class implements the interface ::IF_DmxSliderPanel, with a set
 * of sliders that can be manipulated to change light level and that move
 * autonomously to reveal level change produced by DMX transition engine.
 *
 * This implementation uses Qt Widgets.
 */

#define  ACTIVE_DMX_SLIDER_STYLE \
"QSlider::handle { " \
"background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffdd77, stop:1 #ddcc44);" \
"border: 2px solid yellow;" \
"border-radius: 4px;}"

namespace {

/** a bit higher than DMX max value (255). This allows not loosing precision
 * when manually moving the slider
 */
const int MAX_SLIDER_VALUE = 300;
/** value returned from QList::IndexOf */

/** transform internal steps into percent value */
inline double TICKS_TO_PERCENT( int internalLevel)
{
   return (static_cast<double>(internalLevel) * 100. / static_cast<double>(MAX_SLIDER_VALUE));
}

/** transform percent value into internal steps */
inline int PERCENT_TO_TICKS( double percent)
{
   return (static_cast<int>(percent * static_cast<double>(MAX_SLIDER_VALUE) / 100.));
}

} // namespace


QtDmxSliderPanel::QtDmxSliderPanel(int numOfChannels,
                                   IF_LightTransitionEngine *transitionEngine,
                                   QWidget *parent) :
   IF_DmxSliderPanel(parent),
   m_updateInProgress(false)
{
   QHBoxLayout *layout = new QHBoxLayout( this);
   setLayout( layout );
   layout->setSpacing(0);
   layout->setContentsMargins( 1,1,1,1);

   for (int i = 0; i < numOfChannels; i++)
   {
      LightChannelWidget *slider = new LightChannelWidget( MAX_SLIDER_VALUE, this);
      layout->addWidget( slider);
      slider->setRange( 0, MAX_SLIDER_VALUE);
      slider->setChannelIndex( i+1); /* first channel is 1 */

      connect( slider, SIGNAL(valueChanged(int, int)), this, SLOT(onSliderValueChanged(int, int)) );
      connect( slider, SIGNAL(manualOnlyChanged(bool, int)),
               this, SLOT(onManualOnlyChangedByGUI(bool, int)) );
      m_sliderSet << slider;
   }

   connect( transitionEngine, SIGNAL(fadeStep(QList<double>)),
            this, SLOT(setAllValues(QList<double>)) );
}

const QList<double> & QtDmxSliderPanel::values()
{
   m_values.clear();

   foreach( LightChannelWidget * slider, m_sliderSet)
   {
      m_values.append( TICKS_TO_PERCENT(slider->value()) );
   }

   return m_values;
}

void QtDmxSliderPanel::setAllValues(const QList<double> &values)
{
   T_ASSERT( values.length() == m_sliderSet.length());

   for (int i = 0; i < m_sliderSet.length(); i++)
   {
      m_updateInProgress = true;
      m_sliderSet[i]->setValue( PERCENT_TO_TICKS(values.at(i)) );
      m_updateInProgress = false;
   }
}

void QtDmxSliderPanel::setValueAtChannel(double value, int channel)
{
   T_ASSERT( channel < m_sliderSet.length());

   m_updateInProgress = true;
   m_sliderSet[channel]->setValue( PERCENT_TO_TICKS(value) );
   m_updateInProgress = false;
}

void QtDmxSliderPanel::setDmxOn()
{
   setStyleSheet( ACTIVE_DMX_SLIDER_STYLE);
}

void QtDmxSliderPanel::setDmxOff()
{
   setStyleSheet("");
}

void QtDmxSliderPanel::setManualOnlyChannel( bool manual, int channelIndex)
{
   /* 'channelIndex' is zero based */
   if ((channelIndex >= 0) && (channelIndex < m_sliderSet.size()))
   {
      m_sliderSet[channelIndex]->setManualOnly( manual);
      manualOnlyChanged( manual, channelIndex);
   }
}

void QtDmxSliderPanel::onRequestToCreatePreset()
{
   QList<double> values;

   foreach( LightChannelWidget * channel, m_sliderSet)
   {
      values << TICKS_TO_PERCENT( channel->value());
   }

   emit createPresetFromValues( values);
}

/**
 * called when any of the sliders is moved. This function will
 * propagate the signal with the info of which one did move.
 */
void QtDmxSliderPanel::onSliderValueChanged(int value, int channel)
{
   if ( ! m_updateInProgress)
   {
      T_ASSERT( channel >= 1);
      double newValue = TICKS_TO_PERCENT( value );
      emit valueChangedByGUI( newValue, channel - 1 );
   }
}

void QtDmxSliderPanel::onManualOnlyChangedByGUI(bool isManual, int channel)
{
   if ( ! m_updateInProgress)
   {
      T_ASSERT( channel >= 1);
      /* 'channel' is 1 based, but signal uses channel-index */
      emit manualOnlyChanged( isManual, channel - 1 );
   }
}

