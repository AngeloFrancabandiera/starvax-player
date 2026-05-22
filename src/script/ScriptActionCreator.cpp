#include "ScriptActionCreator.h"
#include <QTextBrowser>
#include <QAbstractListModel>
#include <QMap>
#include "testableAssert.h"

#include "LightPresetData.h"
#include "ActionTargetSelectorDialog.h"
#include "PlaylistDecks.h"


/** html for light event */
#define  LIGHT_ACTION_HTML(label)  \
   QString("<br><h2><a href=\"$$EVENT_LIGHT$$%1\"><span style=\" color: #11ddee; background: #ee4411;\">").arg(label) +\
   QString("<img src=\"file:light.png\"/></a> LIGHT:  %1 </span></h2><br>").arg(label)

/** html for audio/video event */
#define  MEDIA_ACTION_HTML(label, deck)  \
   QString("<br><h2><a href=\"$$EVENT_MEDIA_LINE_%1$$%2\"><span style=\"font-size: 15.0 pt; background: #1188ee;\">").arg(deck).arg(label) +\
   QString("<img src=\"file:sound_A.png\"/></a> MEDIA %1:  %2 </span></h2><br>").arg(deck).arg(label)

/** html for picture event */
#define  PICTURE_ACTION_HTML(label, deck)  \
   QString("<br><h2><a href=\"$$EVENT_PICTURE_LINE_%1$$%2\"><span style=\" background: #1177ff; color: white\">").arg(deck).arg(label) +\
   QString("<img src=\"file:picture_A.png\"/></a> PICTURE %1:  %2 </span></h2><br>").arg(deck).arg(label)

/** html for sequencer entry event */
#define SEQUENCER_ENTRY_HTML(label) \
   QString("<br><h2><a href=\"$$EVENT_SEQUENCER_ENTRY$$%1\"><span style=\" color: #11eeff; background: #ee1144;\">").arg(label) +\
   QString("<img src=\"file:sequencer_play.png\"/></a> SEQUENCE ENTRY:  %1 </span></h2><br>").arg(label)

namespace  {
QMap< AbstractMediaSource::MediaKind, IF_ActionSelectorInterface::Type> MEDIA_KIND_TABLE;

}  // namespace



ScriptActionCreator::ScriptActionCreator(IF_ActionSelectorInterface &actionSelector) :
   m_actionSelector(actionSelector)
{
   MEDIA_KIND_TABLE.insert( AbstractMediaSource::AUDIO_VIDEO,
                            IF_ActionSelectorInterface::MediaAction);
   MEDIA_KIND_TABLE.insert( AbstractMediaSource::PICTURE,
                            IF_ActionSelectorInterface::PictureAction);
}

QString ScriptActionCreator::makeHtmlForLightAction()
{
   m_actionSelector.selectEvent( IF_ActionSelectorInterface::LightAction);
   return makeHtmlForAction();
}

QString ScriptActionCreator::makeHtmlForSequenceAction()
{
   m_actionSelector.selectEvent( IF_ActionSelectorInterface::SequenceEntryAction);
   return makeHtmlForAction();
}

QString ScriptActionCreator::makeHtmlForMediaAction()
{
   m_actionSelector.selectEvent( IF_ActionSelectorInterface::MediaAction);

   return makeHtmlForAction();
}


QString ScriptActionCreator::makeHtmlForLightAction(const QString & label)
{
   m_actionSelector.setActionType( IF_ActionSelectorInterface::LightAction);
   m_actionSelector.setActionId( label);

   return makeHtmlForAction();
}

QString ScriptActionCreator::makeHtmlForMediaAction( AbstractMediaSource::MediaKind kind,
                                                     const QString & label,
                                                     QString deckTag)
{
   IF_ActionSelectorInterface::Type type =
         MEDIA_KIND_TABLE.value( kind, IF_ActionSelectorInterface::NoAction);

   m_actionSelector.setActionType( type);
   m_actionSelector.setActionId( label);
   m_actionSelector.setParam( deckTag);

   return makeHtmlForAction();
}

QString ScriptActionCreator::makeHtmlForSequenceAction(const QString& label)
{
   m_actionSelector.setActionType( IF_ActionSelectorInterface::SequenceEntryAction);
   m_actionSelector.setActionId( label);

   return makeHtmlForAction();
}


QString ScriptActionCreator::makeHtmlForAction()
{
   QString htmlForEvent;
   QString actionId = m_actionSelector.getActionId();

   if (m_actionSelector.getActionType() == IF_ActionSelectorInterface::LightAction)
   {
      htmlForEvent = LIGHT_ACTION_HTML(actionId);
   }
   else if (m_actionSelector.getActionType() == IF_ActionSelectorInterface::MediaAction)
   {
      htmlForEvent = MEDIA_ACTION_HTML(actionId, m_actionSelector.getParam());
   }
   else if (m_actionSelector.getActionType() == IF_ActionSelectorInterface::PictureAction)
   {
      htmlForEvent = PICTURE_ACTION_HTML(actionId, m_actionSelector.getParam());
   }
   else if (m_actionSelector.getActionType() == IF_ActionSelectorInterface::SequenceEntryAction)
   {
      htmlForEvent = SEQUENCER_ENTRY_HTML(actionId);
   }
   else
   {
      // this happens if action is canceled
   }

   return htmlForEvent;
}

