#ifndef ITIMERSERVICE_H
#define ITIMERSERVICE_H

#include "QObject"

/**
 * @brief The TimeoutListener class works like a callback for the
 *        expiration of a timer.
 *        A pointer to an instance of this class is passed to method
 *        ::triggerTimeout of a concrete child of ::ITimerService interface.
 */
class TimeoutListener : public QObject
{
   Q_OBJECT
public:
   TimeoutListener(QObject *parent = nullptr) : QObject(parent)
   {
   }

   virtual ~TimeoutListener() override {}

signals:
   void timeout() const;
};

/**
 * @brief The ITimerService class is an interface for a service that
 *        generates a timeout after a given number of milliseconds
 *        since the invocation
 */
class ITimerService
{
public:
   ITimerService()
   {
   }

   virtual ~ITimerService() {}

public:
   virtual void  triggerTimeout( int millisec, const TimeoutListener *listener) = 0;
   virtual void  stopTimer() = 0;
};

#endif // ITIMERSERVICE_H
