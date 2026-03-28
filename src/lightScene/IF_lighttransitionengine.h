#ifndef IF_LIGHTTRANSITIONENGINE_H
#define IF_LIGHTTRANSITIONENGINE_H

#include <QObject>
#include <QList>

/**
 * @brief IF_LightTransitionEngine is a virtual class that exports a method
 *        to start a transition from the current set of levels to new set.
 *
 * The current set of values is managed by child classes, and it's generally
 * the result of previous transition. This class does not manage at all the
 * user of generated values; it only emits a signal with a new set of values
 * at given times.
 */
class IF_LightTransitionEngine : public QObject
{
   Q_OBJECT
public:
   explicit IF_LightTransitionEngine(QObject *parent = 0) :
      QObject(parent)
   {
   }

   virtual void startTransition( const QList<double> &finalValues, int fadeTimeMs) = 0;
   virtual void stopTransition() = 0;
   virtual void setLevelForChannel( double level, int channel) = 0;

public slots:
   virtual void onManualOnlyChanged( bool isManual, int channel) = 0;

signals:
   void fadeStep(const QList<double> &values);
};

#endif // IF_LIGHTTRANSITIONENGINE_H
