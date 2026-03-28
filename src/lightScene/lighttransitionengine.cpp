#include "lighttransitionengine.h"
#include "qtimer.h"
#include "stdlib.h"
#include "testableAssert.h"

/** @class LightTransitionEngine
  * This subclass of \a IF_LightTransitionEngine calculates a
  * linear fading of an array of numbers and notifies the fading
  * steps after a given time.
  *
  * NOTE: this class is not test driven for the presence of QTimer
  */

#define TRANSITION_STEP_ms    100   /**< time between two signals emission (ms) */


static bool isOutOfRange(double val, double i1, double i2);

LightTransitionEngine::LightTransitionEngine( const QList<double> &startValues, QObject *parent) :
   IF_LightTransitionEngine(parent),
   m_startValue(startValues)
{
   m_finalValue = QList<double>();
   m_currentValue = m_startValue;
   m_totalFadeTimeMs = 0;

   connect( &m_fadeTimer, SIGNAL(timeout()), this, SLOT(onFadeTimerTick()));
}


LightTransitionEngine::LightTransitionEngine(int numOfChannels, QObject *parent) :
   IF_LightTransitionEngine(parent)
{
   m_startValue = QList<double>();
   for (int i = 0; i < numOfChannels; i++)
   {
      m_startValue << 0.;
      m_manualOnly << false;
   }

   m_finalValue = QList<double>();
   m_currentValue = m_startValue;
   m_totalFadeTimeMs = 0;

   connect( &m_fadeTimer, SIGNAL(timeout()), this, SLOT(onFadeTimerTick()));
}


void LightTransitionEngine::startTransition(const QList<double> &final, int fadeTimeMs)
{
   m_startValue = m_currentValue;
   m_finalValue = QList<double>(final);
   m_totalFadeTimeMs = fadeTimeMs;

   /* values below a single transition step are rounded */
   if (m_totalFadeTimeMs < TRANSITION_STEP_ms)
   {
      m_totalFadeTimeMs = TRANSITION_STEP_ms;
   }

   /* start fading operation */
   calculateStepValues();
   m_fadeTimer.stop();  /* in case already running */
   m_fadeTimer.start(TRANSITION_STEP_ms);
}

void LightTransitionEngine::stopTransition()
{
   m_fadeTimer.stop();
}


void LightTransitionEngine::setLevelForChannel(double level, int channel)
{
   T_ASSERT_REP (channel < m_currentValue.length(),
                 QString("invalid light channel: %1").arg(channel));

   m_currentValue[channel] = level;
}

void LightTransitionEngine::onManualOnlyChanged( bool isManual, int channel)
{
   T_ASSERT( channel >= 0);

   if (channel < m_manualOnly.size())
   {
      m_manualOnly[channel] = isManual;
   }
}

/** when timer expires, calculated values are signaled,
  * and (after) new values are calculated for next tick.
  * The check for transition finished is done at the beginning
  * to send the final vlue
  */
void LightTransitionEngine::onFadeTimerTick()
{
   checkForFadeFinished();
   emit fadeStep(m_currentValue);
   calculateStepValues();
}

/** calculate the next step for each element */
void LightTransitionEngine::calculateStepValues()
{
   int numberOfChannels = qMin( m_currentValue.length(), m_finalValue.length());

   for(int i = 0; i < numberOfChannels; i++)
   {
      double start = m_startValue[i];
      double final = m_finalValue[i];

      if (m_manualOnly[i] == false)
      {
         /* channel controlled by automation: calculate new value */
         m_currentValue[i] += (final - start) / (static_cast<double>(m_totalFadeTimeMs)/TRANSITION_STEP_ms);

         /* when current value goes out of range [start final], the
          * transition for this index is over...
          */
         if (isOutOfRange(m_currentValue[i], start, final))
         {
            /* ... so fix it to final value
             */
            m_currentValue[i] = final;
         }
      }
   }
}

/** check if all elements have reached the final value.
  * In positive case, stop the timer.
  */
void LightTransitionEngine::checkForFadeFinished()
{
   bool finished = true;

   for(int i = 0; (i < m_currentValue.length()) && (finished == true); i++)
   {
      if (( ! m_manualOnly[i] ) &&
          (abs(m_currentValue[i] - m_finalValue[i]) > 0.001) )
      {
         finished = false;
      }
   }

   if (finished)
   {
      /* all channels reached final value */
      m_fadeTimer.stop();
   }
}

/** determine if val is out of range [i1 .. i2]; it must
  * work both for i1 >= i2 or i2 <= i1
  */
static bool isOutOfRange(double val, double i1, double i2)
{
   double intervalSpan = qAbs(i1 - i2);
   double i1Span = qAbs(val - i1);
   double i2Span = qAbs(val - i2);

   return (i1Span > intervalSpan) || (i2Span > intervalSpan);
}
