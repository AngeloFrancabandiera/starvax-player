#ifndef QTTIMERSERVICE_H
#define QTTIMERSERVICE_H

#include "QObject"
#include "ITimerService.h"

class TimerServiceImplementation;

class QtTimerService : public QObject, public ITimerService
{
public:
   explicit QtTimerService( QObject *parent = nullptr);

   ~QtTimerService();

public:
   virtual void  triggerTimeout( int millisec, const TimeoutListener *listener);
   virtual void  stopTimer();

private:
   TimerServiceImplementation *m_internal;
};

#endif // QTTIMERSERVICE_H
