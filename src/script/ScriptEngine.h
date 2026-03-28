#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include "ITimerService.h"
#include "IF_ScriptViewerInterface.h"
#include "IF_ScriptEngineInterface.h"

class ScriptEngine : public IF_ScriptEngineInterface
{
   Q_OBJECT
public:
   explicit ScriptEngine( IF_ScriptViewerInterface & viewer, ITimerService & timer,
                           QObject *parent = nullptr);

public slots:
   virtual void playScrollAnimation();
   virtual void pauseScrollAnimation();
   virtual void setAnimationSpeed( int speedIndex);

   virtual void setEditMode(bool edit) {
      m_viewerInterface.setEditMode( edit);
   }

   virtual void setScriptContent( const QString & text) {
      m_viewerInterface.setScriptContent( text);
   }

public:
   virtual void getScriptContent( QString & text) const {
      m_viewerInterface.getScriptContent( text);
   }

   virtual const QList<QAction *> & getActionList() {
      return *m_actionList;
   }

   void setActionList( const QList<QAction *> * actionList) {
      m_actionList = actionList;
   }

signals:
   void activateMediaLineA(const QString &, bool, bool);
   void activateMediaLineB(const QString &, bool, bool);
   void activateLight(const QString &, bool);
   void activateSequenceEntry(const QString &, bool);

   void loadShowRequest( const QString & label);

private:
   IF_ScriptViewerInterface & m_viewerInterface;
   ITimerService & m_blockAnimationTimer;
   TimeoutListener m_timerlistener;
   bool m_animationIsActive;  /**< true when auto-scroll is active */
   int m_speedFactor;
   const QList<QAction *> *m_actionList;

private slots:
   void onAnimationTimerExpired();
   void retriggerAnimation();
};

#endif // SCRIPTENGINE_H
