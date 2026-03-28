#ifndef SCRIPTACTIONTRIGGER_H
#define SCRIPTACTIONTRIGGER_H

#include "QObject"

class ScriptActionTrigger : public QObject
{
   Q_OBJECT
public:
   ScriptActionTrigger( QObject *parent = nullptr) : QObject(parent) {}

signals:
   void activateMediaLineA(const QString &, bool, bool dontStopFlag = false);
   void activateMediaLineB(const QString &, bool, bool dontStopFlag = false);
   void activateLight(const QString &, bool);
   void activateSequenceEntry(const QString &, bool);
};

#endif // SCRIPTACTIONTRIGGER_H
