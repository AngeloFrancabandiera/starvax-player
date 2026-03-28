#include "ScriptActionCreator.h"
#include <QTextBrowser>
#include <QAbstractListModel>
#include <QMap>
#include "testableAssert.h"

#include "LightPresetData.h"
#include "ActionTargetSelectorDialog.h"

/** html for light event */
#define  LIGHT_ACTION_HTML(label)  \
   QString("<br><h2><a href=\"$$EVENT_LIGHT$$%1\"><span style=\" color: #11ddee; background: #ee4411;\">").arg(label) +\
   QString("<img src=\"file:light.png\"/></a> LIGHT:  %1 </span></h2><br>").arg(label)

/** html for audio/video event */
#define  MEDIA_ACTION_LINE_A_HTML(label)  \
   QString("<br><h2><a href=\"$$EVENT_MEDIA_LINE_A$$%1\"><span style=\"font-size: 15.0 pt; background: #1188ee;\">").arg(label) +\
   QString("<img src=\"file:sound_A.png\"/></a> MEDIA:  %1 </span></h2><br>").arg(label)

#define  MEDIA_ACTION_LINE_B_HTML(label)  \
   QString("<br><h2><a href=\"$$EVENT_MEDIA_LINE_B$$%1\"><span style=\" background: #1188ee; color: white\">").arg(label) +\
   QString("<img src=\"file:sound_B.png\"/></a> MEDIA:  %1 </span></h2><br>").arg(label)

/** html for picture event */
#define  PICTURE_ACTION_LINE_A_HTML(label)  \
   QString("<br><h2><a href=\"$$EVENT_PICTURE_LINE_A$$%1\"><span style=\" background: #1177ff; color: white\">").arg(label) +\
   QString("<img src=\"file:picture_A.png\"/></a> PICTURE:  %1 </span></h2><br>").arg(label)

#define  PICTURE_ACTION_LINE_B_HTML(label)  \
   QString("<br><h2><a href=\"$$EVENT_PICTURE_LINE_B$$%1\"><span style=\" background: #1177ff; color: white\">").arg(label) +\
   QString("<img src=\"file:picture_B.png\"/></a> PICTURE:  %1 </span></h2><br>").arg(label)

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
   MEDIA_KIND_TABLE.insert( AbstractMediaSource::AUDIO_VIDEO_LINE_A,
                            IF_ActionSelectorInterface::MediaActionLineA);
   MEDIA_KIND_TABLE.insert( AbstractMediaSource::AUDIO_VIDEO_LINE_B,
                            IF_ActionSelectorInterface::MediaActionLineB);
   MEDIA_KIND_TABLE.insert( AbstractMediaSource::PICTURE_LINE_A,
                            IF_ActionSelectorInterface::PictureActionLineA);
   MEDIA_KIND_TABLE.insert( AbstractMediaSource::PICTURE_LINE_B,
                            IF_ActionSelectorInterface::PictureActionLineB);
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

QString ScriptActionCreator::makeHtmlForMediaAction( Playlist::Line line)
{
   if (line == Playlist::LINE_A)
   {
      m_actionSelector.selectEvent( IF_ActionSelectorInterface::MediaActionLineA);
   }
   else if (line == Playlist::LINE_B)
   {
      m_actionSelector.selectEvent( IF_ActionSelectorInterface::MediaActionLineB);
   }
   else
   {
      T_ASSERT( false);
   }

   return makeHtmlForAction();
}


QString ScriptActionCreator::makeHtmlForLightAction(const QString & label)
{
   m_actionSelector.setActionType( IF_ActionSelectorInterface::LightAction);
   m_actionSelector.setActionId( label);

   return makeHtmlForAction();
}

QString ScriptActionCreator::makeHtmlForMediaAction( AbstractMediaSource::MediaKind kind,
                                                     const QString & label)
{
   IF_ActionSelectorInterface::Type type =
         MEDIA_KIND_TABLE.value( kind, IF_ActionSelectorInterface::NoAction);

   m_actionSelector.setActionType( type);
   m_actionSelector.setActionId( label);

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
   else if (m_actionSelector.getActionType() == IF_ActionSelectorInterface::MediaActionLineA)
   {
      htmlForEvent = MEDIA_ACTION_LINE_A_HTML(actionId);
   }
   else if (m_actionSelector.getActionType() == IF_ActionSelectorInterface::MediaActionLineB)
   {
      htmlForEvent = MEDIA_ACTION_LINE_B_HTML(actionId);
   }
   else if (m_actionSelector.getActionType() == IF_ActionSelectorInterface::PictureActionLineA)
   {
      htmlForEvent = PICTURE_ACTION_LINE_A_HTML(actionId);
   }
   else if (m_actionSelector.getActionType() == IF_ActionSelectorInterface::PictureActionLineB)
   {
      htmlForEvent = PICTURE_ACTION_LINE_B_HTML(actionId);
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

