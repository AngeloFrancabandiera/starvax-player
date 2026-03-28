#ifndef OPENWEBNETENGINE_IF_H
#define OPENWEBNETENGINE_IF_H

#include <QObject>

class OpenWebNetEngine_IF : public QObject
{
   Q_OBJECT
public:
   explicit OpenWebNetEngine_IF( QObject *parent = nullptr) :
      QObject( parent)
   {}

public slots:
   virtual void turnOn(const QString & where) = 0;
   virtual void turnOff(const QString & where) = 0;

   /**
    * @brief send an OWN dimmer command
    * @param level: 1 -> off; 2-10: 20% to 100%
    * @param where is not checked for syntax
    */
   virtual void dimmer( int level, const QString & where) = 0;
};

#endif // OPENWEBNETENGINE_IF_H
