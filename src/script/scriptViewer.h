#ifndef SCRIPTVIEWER_H
#define SCRIPTVIEWER_H

#include <QTextBlock>

#include "qtextcursor.h"
#include <QMouseEvent>

#include "qtimer.h"

#include "IF_ScriptViewerInterface.h"

class ScriptActionCreator;
class HighLightTextViewer;
class SearchTextBar;
class ScriptEditToolbar;


class ScriptViewer : public IF_ScriptViewerInterface
{
   Q_OBJECT
public:
   explicit ScriptViewer( HighLightTextViewer & textViewer,
                          ScriptActionCreator & actionCreator,
                          SearchTextBar & searchBar,
                          ScriptEditToolbar & editToolbar,
                          QObject *parent = nullptr);

   /* IF_ScriptViewerInterface interface */
public:
   void onActivateNextBlock() override;
   int getActiveBlockLength() const override;
   void getScriptContent(QString &content) const override;
   void setScriptContent(const QString &content) override;
   void setEditMode(bool) override;
public slots:
   void addLightEvent() override;
   void addMediaEvent() override;
   void addSeqEntryEvent() override;
   void activateSearchBar() override;
   void setMenuActionList( const QList<QAction *> & actionList) override;
   void addSearchPath(const QString &) override;
   void onLightEventRequested( const QString & label) override;
   void onSequenceEventRequested( const QString & label) override;
   void onMediaEventRequested(AbstractMediaSource::MediaKind kind, QString label) override;
   void markWordUnderCursorAsCharacter() override;


private:
   HighLightTextViewer & m_textViewer;
   ScriptActionCreator & m_actionHtmlMaker;
   SearchTextBar & m_searchBar;
   ScriptEditToolbar & m_editToolbar;
};

#endif // SCRIPTVIEWER_H
