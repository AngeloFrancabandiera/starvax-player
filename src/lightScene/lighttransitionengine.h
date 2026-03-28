#ifndef LIGHTTRANSITIONENGINE_H
#define LIGHTTRANSITIONENGINE_H

#include <QObject>
#include <QList>
#include <QTimer>

#include "IF_lighttransitionengine.h"

class LightTransitionEngine : public IF_LightTransitionEngine
{
   Q_OBJECT
public:
   /** constructor
    * Initial values are passed as a list of doubles. The length of this list
    * will determine the number of channels.
    * @param startValues - list of light sets in range (0 - 100) (percent)
    * @param parent - parent object
    */
   explicit LightTransitionEngine(const QList<double> &startValues, QObject *parent = 0);

   /** constructor
    * Use this constructor if you don't have initial values to specify. Such values
    * will be defaulted to zero.
    * @param numOfChannels - number of DMX channels
    * @param parent - parent object
    */
   explicit LightTransitionEngine(int numOfChannels, QObject *parent = 0);

public:
   /** redefinition of pure virtual function
     * start a transition from current values (internally stored) to
     * values in \p final, generating a signal every ::TRANSITION_STEP_ms
     * milliseconds.
     * @par fadeTimeMs: duration of fade. If 0 or negative, fade does
     *    not start
     * @note The number of channels that will actually be processed is clipped
     *    to the size of the list that was passed to constructor.
     */
   virtual void startTransition( const QList<double> &final, int fadeTimeMs);

   /**
    * @brief stops the transition. Lights are left as they are at the time
    *    of the call
    */
   virtual void stopTransition();

   /** redefinition of pure virtual function
    * manually set the value for one channel, (for example from GUI).
    * @param level - light level in percent of range (0 - 100)
    * @param channel - light channel. Must be in range defined by the lenght of
    *           the list that was passed to constructor.
    */
   virtual void setLevelForChannel(double level, int channel);

public slots:
   /**
    * @brief set a channel as controlable or non-controllable by
    *    preset automation.
    * @param isMaunal is TRUE when channel is NOT controlled by automation
    * @param channel is 0 based
    */
   virtual void onManualOnlyChanged( bool isManual, int channel);

private:
   QList<double> m_finalValue;    /**< target values for a transition */
   QList<double> m_currentValue;  /**< current value for all channels */
   QList<double> m_startValue;    /**< current value sampled at start of transition */
   QList<bool> m_manualOnly;      /**< array of enable-signal per channel */
   int m_totalFadeTimeMs;         /**< duration of current transition */
   QTimer m_fadeTimer;

private slots:
   void onFadeTimerTick();

private:
   void calculateStepValues();
   void checkForFadeFinished();
};

#endif // LIGHTTRANSITIONENGINE_H
