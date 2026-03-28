#include "QtTimerService.h"

#include "qtimer.h"

class TimerServiceImplementation
{
public:
   TimerServiceImplementation() :
      timer(nullptr) {}

   QTimer *timer;
};


QtTimerService::QtTimerService(QObject *parent) :
   QObject(parent),
   ITimerService()
{
   m_internal = new TimerServiceImplementation;
}

QtTimerService::~QtTimerService()
{
   delete m_internal;
}

void QtTimerService::triggerTimeout(int millisec, const TimeoutListener *listener)
{
   if (m_internal->timer)
   {
      delete m_internal->timer;
   }
   m_internal->timer = new QTimer( this);

   connect ( m_internal->timer, SIGNAL(timeout()), listener, SIGNAL(timeout()));
   m_internal->timer->setSingleShot( true);
   m_internal->timer->start( millisec);
}

/**
 * interface function to pause timer.
 */
void QtTimerService::stopTimer()
{
   if (m_internal->timer != nullptr)
   {
      m_internal->timer->stop();

      delete m_internal->timer;
      m_internal->timer = nullptr;
   }
}

