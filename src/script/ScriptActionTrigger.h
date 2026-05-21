#ifndef SCRIPTACTIONTRIGGER_H
#define SCRIPTACTIONTRIGGER_H

#include "QObject"

class ScriptActionTrigger : public QObject
{
   Q_OBJECT
public:
   ScriptActionTrigger( QObject *parent = nullptr) : QObject(parent) {}

signals:
   void activateMedia( const QString & label, const QString & deck,
                       bool playImmediately, bool dontStopFlag = false);
   void activateLight(const QString &, bool);
   void activateSequenceEntry(const QString &, bool);
};

#endif // SCRIPTACTIONTRIGGER_H
