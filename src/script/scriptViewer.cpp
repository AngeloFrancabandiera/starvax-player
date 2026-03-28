#include "scriptViewer.h"

#include "HighLightTextViewer.h"
#include "ScriptActionCreator.h"
#include "SearchTextBar.h"
#include "ScriptEditToolbar.h"



ScriptViewer::ScriptViewer( HighLightTextViewer & textViewer,
                            ScriptActionCreator &actionCreator,
                            SearchTextBar & searchBar,
                            ScriptEditToolbar &editToolbar,
                            QObject *parent) :
   IF_ScriptViewerInterface(parent),
   m_textViewer(textViewer),
   m_actionHtmlMaker(actionCreator),
   m_searchBar( searchBar),
   m_editToolbar( editToolbar)
{
   connect( &m_textViewer, & HighLightTextViewer::textChanged, this, & ScriptViewer::textChanged);
   connect( &m_textViewer, & HighLightTextViewer::insertPlaylistEventRequest,
            this, & ScriptViewer::onMediaEventRequested);
   connect( &m_textViewer, & HighLightTextViewer::insertLightEventRequest,
            this, & ScriptViewer::onLightEventRequested);
   connect( &m_textViewer, & HighLightTextViewer::insertSequenceEntryRequest,
            this, & ScriptViewer::onSequenceEventRequested);
}

/**
 * set as active the block following the current one.
 */
void ScriptViewer::onActivateNextBlock()
{
   m_textViewer.onActivateNextBlock();
}

int ScriptViewer::getActiveBlockLength() const
{
   return m_textViewer.getActiveBlockLength();
}

void ScriptViewer::getScriptContent(QString &content) const
{
   m_textViewer.getScriptContent( content);
}

void ScriptViewer::setScriptContent(const QString &content)
{
   m_textViewer.setScriptContent( content);
}

void ScriptViewer::setEditMode(bool edit)
{
   m_textViewer.setEditMode( edit);
   m_editToolbar.setVisible( edit);
}

void ScriptViewer::addLightEvent()
{
   m_textViewer.insertHtml( m_actionHtmlMaker.makeHtmlForLightAction() );
}

void ScriptViewer::setMenuActionList(const QList<QAction *> &actionList)
{
   m_textViewer.addActions( actionList);
}

void ScriptViewer::addMediaEvent()
{
   /* always start from page of LINE A; in case, user will switch later to LINE B */
   m_textViewer.insertHtml( m_actionHtmlMaker.makeHtmlForMediaAction( Playlist::LINE_A) );
}

void ScriptViewer::addSeqEntryEvent()
{
   QString html = m_actionHtmlMaker.makeHtmlForSequenceAction();
   m_textViewer.insertHtml( html);
}

void ScriptViewer::activateSearchBar()
{
   m_searchBar.activateBar();
}

void ScriptViewer::addSearchPath(const QString &path)
{
   m_textViewer.setSearchPaths( m_textViewer.searchPaths()<<path );
}

void ScriptViewer::onLightEventRequested( const QString & label)
{
   QString html = m_actionHtmlMaker.makeHtmlForLightAction( label);
   m_textViewer.insertHtml( html);
}

void ScriptViewer::onSequenceEventRequested(const QString &label)
{
   QString html = m_actionHtmlMaker.makeHtmlForSequenceAction( label);
   m_textViewer.insertHtml( html);
}

void ScriptViewer::onMediaEventRequested( AbstractMediaSource::MediaKind kind, QString label)
{
   QString html = m_actionHtmlMaker.makeHtmlForMediaAction( kind, label);
   m_textViewer.insertHtml( html);
}

void ScriptViewer::markWordUnderCursorAsCharacter()
{
   m_textViewer.markWordUnderCursorAsCharacter();
}

