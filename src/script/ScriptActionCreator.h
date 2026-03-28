#ifndef SCRIPTACTIONCREATOR_H
#define SCRIPTACTIONCREATOR_H

#include "QString"
#include "PlaylistConstants.h"
#include "AbstractMediaSource.h"

class QTextBrowser;
class IF_ActionSelectorInterface;

class ScriptActionCreator
{
public:
   ScriptActionCreator( IF_ActionSelectorInterface & actionSelector);

public:
   QString makeHtmlForLightAction();
   QString makeHtmlForSequenceAction();
   QString makeHtmlForMediaAction(Playlist::Line line);
   QString makeHtmlForLightAction( const QString & label);
   QString makeHtmlForMediaAction( AbstractMediaSource::MediaKind kind, const QString & label);
   QString makeHtmlForSequenceAction( const QString & label);

private:
   IF_ActionSelectorInterface & m_actionSelector;

private:
   QString makeHtmlForAction();
};

#endif // SCRIPTACTIONCREATOR_H
