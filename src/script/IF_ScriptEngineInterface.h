#ifndef IF_SCRIPTENGINEINTERFACE_H
#define IF_SCRIPTENGINEINTERFACE_H

#include "qobject.h"
#include "QList"

class QAction;

class IF_ScriptEngineInterface : public QObject
{
   Q_OBJECT
public:
   IF_ScriptEngineInterface( QObject *parent = nullptr) : QObject(parent) {}
   virtual ~IF_ScriptEngineInterface() {}

public slots:
   virtual void playScrollAnimation() = 0;
   virtual void pauseScrollAnimation() = 0;

   virtual void setEditMode( bool) = 0;
   virtual void setAnimationSpeed( int speedIndex) = 0;

   virtual void setScriptContent( const QString & text) = 0;
public:
   virtual void getScriptContent( QString & text) const = 0;

   virtual const QList<QAction *> & getActionList() = 0;

signals:
   void scriptIsAnimated(bool);
   void textChanged();
   void newSearchPath(const QString &);
   void addLightEventRequest();
   void addMediaEventRequest();
   void addSeqEntryEventRequest();
   void findTextRequest();
   void markAsCharacterRequest();
};

#endif // IF_SCRIPTENGINEINTERFACE_H
