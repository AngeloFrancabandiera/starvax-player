#include "LightsetListView.h"

#include <QDropEvent>
#include <QMimeData>

#include "StatusDisplay.h"
#include "ActionListController.h"
#include "modelViewRules.h"
#include "testableAssert.h"
#include "LightPresetModel.h"

namespace  {
const int END_OF_LIST = -1;
}  // namespace


LightsetListView::LightsetListView( ActionListController & actionController,
                                    ActionListPainter * painter,
                                    StatusDisplay & msgDisplay,
                                    QWidget * parent) :
   ActionListView( actionController, painter,
                   msgDisplay, parent),
   m_actionController( actionController),
   m_msgDisplay( msgDisplay),
   m_insertPosition( END_OF_LIST)
{
   setDragEnabled( true);
   setAcceptDrops(true);

}


void LightsetListView::dragEnterEvent( QDragEnterEvent * event)
{
   if (event->mimeData()->hasFormat("lightPreset-data"))
   {
      m_insertPosition = indexAt( event->position().toPoint()).row(); // just in case of immediate drop
      event->accept();
   }
}

void LightsetListView::dragMoveEvent(QDragMoveEvent * event)
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


void LightsetListView::dragLeaveEvent(QDragLeaveEvent * event)
{
   m_insertPosition = END_OF_LIST;
   setInsertIndex( m_insertPosition);
   repaint();
   event->accept();
}

void LightsetListView::dropEvent(QDropEvent * event)
{
   if (isEditMode())
   {
      if (event->mimeData()->hasFormat("lightPreset-data"))
      {
         /* this can only happen in internal move */
         moveDroppedItems( event);
         m_insertPosition = END_OF_LIST;
         setInsertIndex( m_insertPosition);
         event->accept();
      }
      else
      {
         m_msgDisplay.showMessage( tr("Light pane only accepts internal move"),
                                   StatusDisplay::WARNING);
      }
   }
   else
   {
      m_msgDisplay.showMessage( tr("Enter <b>edit mode</b> to move light presets."),
                                StatusDisplay::WARNING);
   }

}

void LightsetListView::moveDroppedItems( QDropEvent * event)
{
   /* this is an internal move. Act on model's item instead
    * of destroying and creating new items */
   m_insertPosition = indexAt( event->position().toPoint()).row();

   QModelIndexList selection = selectionModel()->selectedIndexes();
   m_actionController.moveItemsRequest( selection, m_insertPosition);

   selectionModel()->select( QModelIndex(), QItemSelectionModel::Clear);
}

