#ifndef IF_LIGHTENGINEINTERFACE_H
#define IF_LIGHTENGINEINTERFACE_H

#include <QObject>

class IF_LightEngineInterface : public QObject
{
   Q_OBJECT
public:
   IF_LightEngineInterface( QObject *parent = nullptr): QObject(parent) {}
   virtual ~IF_LightEngineInterface() {}

   /** start transition with call to \a activateByName */
   static const bool START_NOW = true;

   /** do not start transition with call to \a activateByName */
   static const bool DONT_START_NOW = false;

public slots:
   virtual void requestDmxStart() = 0;
   virtual void requestDmxStop() = 0;

   virtual void setLevelForChannel(double level, int channel) = 0;
   virtual void activateByName( const QString &label, bool triggerNow) = 0;
   virtual void triggerTransition( const QString &label) = 0;
   virtual void freezeTransition() = 0;

signals:
   void lightsetChanged(const QList<double> &newValues);
   void dmxStarted();
   void dmxStopped();

   /**
    * @brief facility signal emitted together with dmxStarted (with
    * param true) of dmxStopped (with param false);
    */
   void dmxStartChanged(bool);
};

#endif // IF_LIGHTENGINEINTERFACE_H
