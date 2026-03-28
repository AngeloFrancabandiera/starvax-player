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

   m_actionTypeForLabel["EVENT_MEDIA"] = ScriptActionLauncher::audVidAction_lineA;  // legacy
   m_actionTypeForLabel["EVENT_MEDIA_LINE_A"] = ScriptActionLauncher::audVidAction_lineA;
   m_actionTypeForLabel["EVENT_MEDIA_LINE_B"] = ScriptActionLauncher::audVidAction_lineB;

   m_actionTypeForLabel["EVENT_PICTURE"] = ScriptActionLauncher::pictureAction_lineA;  // legacy
   m_actionTypeForLabel["EVENT_PICTURE_LINE_A"] = ScriptActionLauncher::pictureAction_lineA;
   m_actionTypeForLabel["EVENT_PICTURE_LINE_B"] = ScriptActionLauncher::pictureAction_lineB;

   m_actionTypeForLabel["EVENT_LIGHT"] = ScriptActionLauncher::lightAction;
   m_actionTypeForLabel["EVENT_SEQUENCER_ENTRY"] = ScriptActionLauncher::sequencerEntryAction;
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
      QStringList anchorData = cursor.charFormat().anchorHref().split("$$", STR_SPLIT::SkipEmptyParts);

      if (anchorData.length() >= 2)
      {
         type = detectActionType(anchorData);

         m_actionLauncher->setActionType( type);
         m_actionLauncher->setActionName( formatActionLabel( type, anchorData.at(1)));
      }
      else
      {
         m_actionLauncher->setActionType( ScriptActionLauncher::invalidAction);
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
   ScriptActionLauncher::ActionType type = m_actionTypeForLabel.value(anchorData.at(0),
                                                                      ScriptActionLauncher::invalidAction);
   return type;
}

QString ActionHoverDetection::formatActionLabel( ScriptActionLauncher::ActionType type, const QString & fullData)
{
   QString label;

   if ((type == ScriptActionLauncher::audVidAction_lineA) ||
       (type == ScriptActionLauncher::audVidAction_lineB) ||
       (type == ScriptActionLauncher::pictureAction_lineA) ||
       (type == ScriptActionLauncher::pictureAction_lineB))
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

