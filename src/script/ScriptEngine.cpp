#include "ScriptEngine.h"
#include "testableAssert.h"

/** conversion factor (empirical!) from block length (in characters)
 * and animation speed (in milliseconds)
 */
#define  MILLISEC_PER_CHAR      5

/** Look up table from int index to a speed factor with logaritmic grow
  */
static int  SPEED_FACTOR[] = { 1, 2, 5, 10, 20, 50, 100 };

#define DEFAULT_SPEED_INDEX   3 /**< referred to ::SPEED_FACTOR */

ScriptEngine::ScriptEngine(IF_ScriptViewerInterface & viewer, ITimerService & timer,
                           QObject *parent) :
   IF_ScriptEngineInterface(parent),
   m_viewerInterface(viewer),
   m_blockAnimationTimer(timer),
   m_timerlistener(this),
   m_speedFactor(SPEED_FACTOR[3]),
   m_actionList(nullptr)
{
   connect( &m_timerlistener, SIGNAL(timeout()), this, SLOT(onAnimationTimerExpired()) );
}

void ScriptEngine::playScrollAnimation()
{
   m_animationIsActive = true;
   retriggerAnimation();

   emit scriptIsAnimated( true);
}

void ScriptEngine::pauseScrollAnimation()
{
   m_animationIsActive = false;
   m_blockAnimationTimer.stopTimer();

   emit scriptIsAnimated( false);
}

/**
 * set speed of auto scroll animation
 * @param speedIndex - index (o based) of a logaritmic convertion table
 */
void ScriptEngine::setAnimationSpeed(int speedIndex)
{
   T_ASSERT( speedIndex >= 0 );
   T_ASSERT( static_cast<unsigned>(speedIndex) < (sizeof(SPEED_FACTOR) / sizeof(int)) );

   m_speedFactor = SPEED_FACTOR[speedIndex];
}

void ScriptEngine::onAnimationTimerExpired()
{
   m_viewerInterface.onActivateNextBlock();
   retriggerAnimation();
}

void ScriptEngine::retriggerAnimation()
{
   /* stop current timing ... */
   m_blockAnimationTimer.stopTimer();

   /* ... in case, start another */
   if (m_animationIsActive)
   {
      int block_time;

      /* calculate "duration" of active block */
      block_time = m_viewerInterface.getActiveBlockLength()*MILLISEC_PER_CHAR*m_speedFactor;

      /* timer should be stopped */
      m_blockAnimationTimer.triggerTimeout( block_time, &m_timerlistener );
   }
}

