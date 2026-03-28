#include "MediaListView.h"
#include "supported_files.h"
#include "AbstractMediaSource.h"
#include "modelViewRules.h"
#include "ActionListController.h"
#include "StatusDisplay.h"
#include "MediaListModel.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>

namespace  {
QStringList SUPPORTED_MEDIA_EXTENTION = QStringList() << MEDIA_TRACKS_SUPPORTED_FORMATS << PICTURE_SUPPORTED_FORMATS;
const int END_OF_LIST = -1;  /**< invalid value for index row */
}  // namespace


MediaListView::MediaListView( ActionListController & actionController,
                              ActionListPainter * painter,
                              StatusDisplay & msgDisplay, QWidget * parent) :
   ActionListView( actionController, painter, msgDisplay, parent),
   m_actionListController( actionController),
   m_msgDisplay( msgDisplay),
   m_insertPosition(END_OF_LIST),
   m_localDragFlag( false)
{
   setDragEnabled( true);
   setAcceptDrops( true);
}

void MediaListView::startDrag(Qt::DropActions supportedActions)
{
   /* set start flag before calling base class.
    * Drop action is within the context of the call
    * to 'startDrag'
 */
   m_localDragFlag = true;
   ActionListView::startDrag( supportedActions);
   m_localDragFlag = false;
}


void MediaListView::dragEnterEvent( QDragEnterEvent * event)
{
   if (event->mimeData()->hasUrls())
   {
      bool hasValidUrl = dragHasValidUrl(event);

      if (hasValidUrl)
      {
         event->accept();
      }
   }

   if (event->mimeData()->hasFormat("playlist-data"))
   {
      event->accept();
   }
}


void MediaListView::dragMoveEvent( QDragMoveEvent * event)
{
   if (isEditMode())
   {
      m_insertPosition = indexAt( event->position().toPoint()).row();
      /* visual feedback */
      setInsertIndex( m_insertPosition);

      repaint();
      event->accept();
   }
}

void MediaListView::dragLeaveEvent( QDragLeaveEvent * event)
{
   setInsertIndex( -1);
   repaint();
   event->accept();
}


void MediaListView::dropEvent( QDropEvent * event)
{
   if (isEditMode())
   {
      handleDropOperation(event);
      event->accept();
   }
   else
   {
      m_msgDisplay.showMessage(tr("Enter <b>edit mode</b> to drop media."), StatusDisplay::WARNING);
   }
}

void MediaListView::handleDropOperation(QDropEvent* event)
{
   selectDropOperation(event);

   if (event->mimeData()->hasUrls())
   {
      /* drop of files from external applications */
      addValidUrlsToModel( event);
   }

   /* for local drag-n-drop, remove old elements before adding new ones */
   if (m_localDragFlag)
   {
      /* this is an internal move. Act on model's item instead
       * of destroying and creating new items */
      m_insertPosition = indexAt( event->position().toPoint()).row();

      QModelIndexList selection = selectionModel()->selectedIndexes();
      m_actionListController.moveItemsRequest( selection, m_insertPosition);

      selectionModel()->select( QModelIndex(), QItemSelectionModel::Clear);
   }
   else
   {
      /* may be a drag-n-drop from the other line */
      if (event->mimeData()->hasFormat("playlist-data"))
      {
         addDroppedMedia( event);
      }
   }

   setInsertIndex( END_OF_LIST);
}

void MediaListView::selectDropOperation(QDropEvent* event)
{
   /* select 'move' operation by holding SHIFT when dropping.
    * When drag-n-drop is within the same playlist, a 'copy' operation
    * is selected because native 'move' operation creates the new elements
    * before removing the old ones, resulting in wrong labels */

   bool shiftPressed = ((event->modifiers() & Qt::ShiftModifier) != 0);

   event->setDropAction( Qt::CopyAction);

   if ( (event->possibleActions() & Qt::MoveAction) &&
        shiftPressed && ( ! m_localDragFlag) )
   {
      event->setDropAction( Qt::MoveAction);
   }
   else
   {
      if (m_localDragFlag == false)
      {
         m_msgDisplay.showMessage( tr("Copying items. Hold SHIFT to move."),
                                   StatusDisplay::INFORMATION);
      }
   }
}

bool MediaListView::dragHasValidUrl( QDragEnterEvent* event)
{
   bool hasValidUrl = false;

   foreach (QUrl url, event->mimeData()->urls())
   {
      if (SUPPORTED_MEDIA_EXTENTION.contains( QFileInfo(url.toLocalFile()).suffix()) )
      {
         hasValidUrl = true;
         break;
      }
   }

   return hasValidUrl;
}

void MediaListView::addValidUrlsToModel( QDropEvent * event)
{
   foreach (QUrl url, event->mimeData()->urls())
   {
      if (SUPPORTED_MEDIA_EXTENTION.contains( QFileInfo(url.toLocalFile()).suffix()))
      {
         model()->setData( model()->index(m_insertPosition,0),
                           QVariant::fromValue<QString>(url.toLocalFile()),
                           modelViewRules::CreateFromString );

         updateInsertPosition();
      }
   }
}


void MediaListView::addDroppedMedia(QDropEvent* event)
{
   event->accept();

   QByteArray encodedData = event->mimeData()->data("playlist-data");
   QDataStream stream( & encodedData, QIODevice::ReadOnly);

   /* media has been dropped from the other pane. We only care
    * about media URL path; the rest will be defined in this pane */
   int kind;  // AbstractMediaSource::MediaKind
   QString label;
   QString fileName;

   while ( ! stream.atEnd())
   {
      stream >> kind;
      stream >> label;
      stream >> fileName;

      /* with 'CreateFromString' rule, a new item is created in model
       * before setting data for it */
      model()->setData( model()->index(m_insertPosition,0),
                        QVariant::fromValue<QString>(fileName),
                        modelViewRules::CreateFromString );
      updateInsertPosition();
   }
}

void MediaListView::updateInsertPosition()
{
   /* increment only if index is valid */
   if (m_insertPosition != END_OF_LIST)
   {
      m_insertPosition ++;
   }
}
