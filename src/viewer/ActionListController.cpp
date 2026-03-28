#include "ActionListController.h"
#include <QModelIndex>
#include "ExtAbstractListModel.h"
#include "testableAssert.h"

const int ActionListController::NoItemActive = -1;
const bool ActionListController::PLAY_IMMEDIATE = true;


ActionListController::ActionListController(ExtAbstractListModel &model, QObject *parent) :
   QObject(parent),
   m_activeItemRow( NoItemActive),
   m_model(model)
{
   connect( &model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(onRowsRemoved(QModelIndex,int,int)) );
}

void ActionListController::activateRequest(const QModelIndex &item, bool playImmediate)
{
   activateRequest( item.row(), playImmediate);
}


void ActionListController::activateRequest(int itemRow, bool playImmediate, bool dontStopFlag)
{
   if (m_activeItemRow != itemRow)
   {
      m_activeItemRow = itemRow;
      emit activeRowChanged( m_activeItemRow, dontStopFlag);
   }

   /* flag 'play immediate' is valid even for the item already active */
   if (playImmediate)
   {
      emit requestToPlay();
   }
}

/**
 * move element at row \p itemRow one position up.
 * On success, emit a signal with new position of item
 */
void ActionListController::shiftItemUp(int itemRow)
{
   bool result = m_model.moveRow( QModelIndex(), itemRow, QModelIndex(), itemRow - 1);

   if (result == true)
   {
      emit shiftedUp( itemRow - 1);

      /* did this change active item ? */
      if (itemRow == m_activeItemRow)
      {
         /* active item moved */
         activateRequest( m_activeItemRow - 1);
      }
      else if (itemRow == (m_activeItemRow + 1))
      {
         /* active item was replaced by the one below*/
         activateRequest( m_activeItemRow + 1);
      }
   }
}

void ActionListController::shiftItemDown(int itemRow)
{
   bool result = m_model.moveRow( QModelIndex(), itemRow, QModelIndex(), itemRow + 1 );

   if (result == true)
   {
      emit shiftedDown( itemRow + 1);

      /* did this change active item ? */
      if (itemRow == m_activeItemRow)
      {
         /* active item moved */
         activateRequest( m_activeItemRow + 1);
      }
      else if (itemRow == (m_activeItemRow - 1))
      {
         /* active item was replaced by the one below*/
         activateRequest( m_activeItemRow - 1);
      }
   }
}

/**
 * model has just removed some rows. Check if this has changed active item.
 * @param fisrt - first index that has been removed. Referred to model before change
 * @param last - last index that has been removed. Referred to model before change
 */
void ActionListController::onRowsRemoved( const QModelIndex &, int first, int last)
{
   if (m_activeItemRow != NoItemActive)
   {
      if ((m_activeItemRow >= first) && (m_activeItemRow <= last))
      {
         activateRequest( NoItemActive);
      }
      else if (m_activeItemRow >= first)
      {
         activateRequest( m_activeItemRow - ((last - first) + 1));
      }
   }
}


void ActionListController::removeItemsRequest( QList<QModelIndex> &indexList)
{
   /* sort element so they can be removed from the last,
    * thus preserving indexes during operation */
   std::sort( indexList.begin(), indexList.end());

   /* remove one item per time, starting from last */
   for(int i = indexList.count() - 1; i >= 0; --i)
   {
      int row = indexList.at(i).row();

      /* remove row in model */
      m_model.removeRow( row );
   }
}


void ActionListController::moveItemsRequest(QList<QModelIndex> & indexList, int insertPosition)
{
   T_ASSERT( insertPosition < m_model.rowCount());

   if (insertPosition == -1)
   {
      /* this happens when dragging after last element */
      insertPosition = m_model.rowCount();
   }

   m_model.beginUndoMacro("move rows");
   m_model.rearrangeItems( indexList, insertPosition);
   m_model.endUndoMacro();
}
