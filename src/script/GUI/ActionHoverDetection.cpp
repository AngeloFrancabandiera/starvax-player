#include "ActionHoverDetection.h"
#include "ScriptActionLauncher.h"
#include "QTextBrowser"
#include "QEvent"
#include "qscrollbar.h"
#include "testableAssert.h"


ActionHoverDetection::ActionHoverDetection( ScriptActionLauncher * actionLauncher,
                                            QTextBrowser *parent) :
   QObject(parent),
   m_viewer(parent),
   m_actionLauncher(actionLauncher)
{
   m_actionLauncher->setVisible( false);
   m_actionLauncher->installEventFilter( this);
}

ActionHoverDetection::~ActionHoverDetection()
{
}


/**
 * when mouse has been moved and it's stopped for a while, "Action Launch" dialog
 * must be shown, but only if mouse is still in viewer area
 * @param position - mouse position on last move event. It may not be actual mouse position
 *                   (if mouse is outside viewer area).
 */
void ActionHoverDetection::onMouseMoveFiltered(const QPoint &position)
{
   QTextCursor cursor = m_viewer->cursorForPosition( position );

   /* if current mouse position is not the same as last move event,
    * then it was moved out of viewer*/
   bool mouseIsStillInViewer = (position == m_viewer->mapFromGlobal(QCursor::pos()) );

   if (mouseIsStillInViewer && cursorIsOverEventIcon( cursor))
   {
      ScriptActionLauncher::ActionType type;
      QString action_param;
      QStringList anchorData = cursor.charFormat().anchorHref().split("$$", Qt::SkipEmptyParts);

      if (anchorData.length() >= 2)
      {
         type = detectActionType(anchorData);
         action_param = detectActionParam(anchorData.first());

         m_actionLauncher->setActionType( type, action_param);
         m_actionLauncher->setActionName( formatActionLabel( type, anchorData.at(1)));
      }
      else
      {
         m_actionLauncher->setActionType( ScriptActionLauncher::invalidAction, "");
         m_actionLauncher->setActionName( tr("[Invalid action associated to icon]"));
      }

      m_actionLauncher->setGeometry( 0, m_viewer->cursorRect(cursor).top(),
                                     m_viewer->width(), m_actionLauncher->height());
      m_actionLauncher->setVisible(true);
   }

}

bool ActionHoverDetection::cursorIsOverEventIcon( const QTextCursor & cursor)
{
   return cursor.charFormat().isAnchor() &&
         cursor.charFormat().anchorHref().startsWith("$$EVENT");
}

ScriptActionLauncher::ActionType ActionHoverDetection::detectActionType(const QStringList & anchorData)
{
   ScriptActionLauncher::ActionType type = ScriptActionLauncher::invalidAction;
   const QString & tag0 = anchorData.at(0);

   if (tag0.startsWith("EVENT_MEDIA"))  // legacy "EVENT_MEDIA" or "EVENT_MEDIA_LINE_x"
   {
      type = ScriptActionLauncher::audioVidAction;
   }
   else if (tag0.startsWith("EVENT_PICTURE"))
   {
      type = ScriptActionLauncher::pictureAction;
   }
   else if (tag0 == "EVENT_LIGHT")
   {
      type = ScriptActionLauncher::lightAction;
   }
   else if (tag0 == "EVENT_SEQUENCER_ENTRY")
   {
      type = ScriptActionLauncher::sequencerEntryAction;
   }
   else
   {
      type = ScriptActionLauncher::invalidAction;
   }

   return type;
}

QString ActionHoverDetection::detectActionParam( const QString & ext_type)
{
   QString param;
   // so far, the only parameter is in media event: it's the last letter of the tag
   if (ext_type == "EVENT_MEDIA")
   {
      param = "A";  // legacy format
   }
   else if (ext_type == "EVENT_PICTURE")
   {
      param = "A";  // legacy format
   }
   if (ext_type.startsWith("EVENT_MEDIA_LINE_"))
   {
      param = ext_type.last(1);
   }
   else if (ext_type.startsWith("EVENT_PICTURE_LINE_"))
   {
      param = ext_type.last(1);
   }

   return param;
}

QString ActionHoverDetection::formatActionLabel( ScriptActionLauncher::ActionType type, const QString & fullData)
{
   QString label;

   if ((type == ScriptActionLauncher::audioVidAction) ||
       (type == ScriptActionLauncher::pictureAction) )
   {
      /* don't show full path; only name with extention */
      label  = fullData.split('/').last();
   }
   else if ((type == ScriptActionLauncher::lightAction) ||
            (type == ScriptActionLauncher::sequencerEntryAction) )
   {
      label = fullData;
   }
   else
   {
      T_ASSERT(false);
   }

   return label;
}


bool ActionHoverDetection::eventFilter(QObject *target, QEvent *event)
{
   if (event->type() == QEvent::Leave)
   {
      /* hide action launcher when mouse is out of launcher itself */
      if (target == m_actionLauncher)
      {
         /* for an obsure reason, when laucher is hidden, focus goes to the
          * first action icon. As a work around, script position is set back.
          */
         int veticalScroll = m_viewer->verticalScrollBar()->sliderPosition();

         m_actionLauncher->setVisible( false);
         m_viewer->verticalScrollBar()->setSliderPosition( veticalScroll);
      }
   }

   return QObject::eventFilter( target, event);
}

