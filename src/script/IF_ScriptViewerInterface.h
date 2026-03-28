#ifndef IF_SCRIPTVIEWERINTERFACE_H
#define IF_SCRIPTVIEWERINTERFACE_H

#include "qobject.h"
#include "AbstractMediaSource.h"

class QString;
class QAction;

/**
 * This interface has the operations that can be done with/to
 * a script viewer. Those are:
 *
 * - control the sequence of paragraph
 * - get data on current paragraph
 * - set or retrieve Html content
 */
class IF_ScriptViewerInterface : public QObject
{
   Q_OBJECT
public:
   IF_ScriptViewerInterface( QObject *parent = nullptr) : QObject(parent){}

public:
   virtual void onActivateNextBlock() = 0;
   virtual int getActiveBlockLength() const = 0;

   virtual void getScriptContent( QString & content) const = 0;
   virtual void setScriptContent( const QString & content) = 0;

   virtual void setEditMode( bool) = 0;
   virtual void setMenuActionList( const QList<QAction *> & actionList) = 0;

public slots:
   /* These seem core functions, but Qt viewer holds text document */
   virtual void addLightEvent() = 0;
   virtual void addMediaEvent() = 0;
   virtual void addSeqEntryEvent() = 0;
   virtual void addSearchPath( const QString &) = 0;
   virtual void activateSearchBar() = 0;
   virtual void markWordUnderCursorAsCharacter() = 0;

   virtual void onLightEventRequested( const QString & label) = 0;
   virtual void onMediaEventRequested( AbstractMediaSource::MediaKind kind, QString label) = 0;
   virtual void onSequenceEventRequested( const QString & label) = 0;

signals:
   void textChanged();
   void loadShowRequest( const QString & label);
};

#endif // IF_SCRIPTVIEWERINTERFACE_H
