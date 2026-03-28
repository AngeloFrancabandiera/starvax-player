#include <ActionListView.h>

#include <QPaintEvent>
#include <QPainter>
#include <QRect>

#include <qscrollbar.h>
#include <QItemSelection>
#include <QPushButton>
#include <QMessageBox>
#include <QAction>

#include "ActionListController.h"
#include "modelViewRules.h"
#include "StatusDisplay.h"
#include "testableAssert.h"

/** @{ compile-time settings */
#define DEFAULT_ITEM_HEIGHT     30
#define DEFAULT_CONTROLLER_HEIGHT   40
/** @} */

/** @{ position of the controller referenced to the area ot the active item */
#define CONTROLLER_HOR_OFFSET    5
#define CONTROLLER_VERT_OFFSET   (DEFAULT_ITEM_HEIGHT)
/* @} */

/** @class ActionListView
 * This class is a generic pseudo-list viewer, where elements are placed vertically and all
 * have the same height except for the "active element", which is taller and has an "action bar".
 * The action bar is a widget, (that is passed by ::setActionBar), with some action controls,
 * (hence the name ActionListView).
 * This class has no business of what control widget does.
 *
 * There are also a couple of buttons in the focused item (that normally is not the active one):
 * these allow item to be shifted up or down. The same can be achived with CTRL+up or CTRL+down.
 *
 */

/** nullptr object for m_controller */
static QWidget *fakeController;

ActionListView::ActionListView( ActionListController &actionController,
                                ActionListPainter *painter,
                                StatusDisplay & msgDisplay, QWidget *parent) :
   QAbstractItemView(parent),
   m_actionController(actionController),
   m_painter( painter),
   m_msgDisplay( msgDisplay),
   m_editMode( false),
   m_insertIndex( INVALID_ITEM),
   m_deleteSelectionAction( new QAction(tr("delete selection"), this)),
   m_activateSelectedAction( new QAction(tr("activate selected"), this)),
   m_activateAndPlaySelectedAction( new QAction(tr("activate and play selected"), this)),
   m_hidePlaybarAction( new QAction(tr("hide playbar"), this))
{
   /* created outside factory because it's not a real collaborator */
   fakeController = new QWidget();
   fakeController->setParent(this);
   fakeController->setGeometry( 0, 0, 0, 0);
   m_actionBar = fakeController;

   /* subclasses are supposed to refedine these values */
   m_itemHeight = DEFAULT_ITEM_HEIGHT;
   m_controllerHeigth = DEFAULT_CONTROLLER_HEIGHT;

   resetStyle();
   setEditMode( false);

   /* for this family of views a double click changes the items that
    * has the controller, so does not edit  */
   setEditTriggers( QAbstractItemView::EditKeyPressed );

   setContextMenuPolicy( Qt::ActionsContextMenu);

   connect( &m_actionController, & ActionListController::activeRowChanged, this, & ActionListView::invalidateGeometries );
   connect( &m_actionController, & ActionListController::shiftedUp, this, & ActionListView::onShifted );
   connect( &m_actionController, & ActionListController::shiftedDown, this, & ActionListView::onShifted );

   /* local actions with shortcuts depending on focused widget */
   m_deleteSelectionAction->setShortcut( Qt::Key_Delete);
   m_deleteSelectionAction->setShortcutContext( Qt::WidgetWithChildrenShortcut);
   this->addAction( m_deleteSelectionAction);
   connect( m_deleteSelectionAction, SIGNAL(triggered()),
            this, SLOT(requestToDeleteSelection()) );

   m_activateSelectedAction->setShortcut( Qt::Key_Return);
   m_activateSelectedAction->setShortcutContext( Qt::WidgetWithChildrenShortcut);
   this->addAction( m_activateSelectedAction);
   connect( m_activateSelectedAction, SIGNAL(triggered()),
            this, SLOT(onActivateSelectedRequest()) );

   m_activateAndPlaySelectedAction->setShortcut( Qt::Key_Enter);
   m_activateAndPlaySelectedAction->setShortcutContext( Qt::WidgetWithChildrenShortcut);
   this->addAction( m_activateAndPlaySelectedAction);
   connect( m_activateAndPlaySelectedAction, SIGNAL(triggered()),
            this, SLOT(onActivateAndPlaySelectedRequest()) );

   this->addAction( m_hidePlaybarAction);
   connect( m_hidePlaybarAction, & QAction::triggered,
            this, & ActionListView::onHidePlaybarRequested);

   setDragEnabled(false);
   viewport()->setAcceptDrops(false);
}

ActionListView::~ActionListView()
{
}

/**
 * @brief reset default color scheme for the view
 */
void ActionListView::resetStyle()
{
   m_painter->resetStyle();
}


/**
 * @param index - entry of the view
 * @return the area occupied by an item in viewport coordinates
 */
QRect ActionListView::visualRect(const QModelIndex &index) const
{
   QRect visRect;

   // only column 0 is valid
   if( index.column() == 0)
   {
      // get rect by row number
      visRect = visualRectFromRow( index.row() );
   }
   else
   {
      visRect = QRect();
   }

   return visRect;
}


QRect ActionListView::visualRectFromRow( int row ) const
{
   QRect visRect;

   if ((row != ActionListController::NoItemActive) && (row < model()->rowCount()))
   {
      int vpRectTop;            /* rect top value in viewport coordinates */
      int height = m_itemHeight;  /* item rect height */
      int activeRow = m_actionController.activeItemRow();

      // base top value
      vpRectTop = (row * m_itemHeight) - verticalScrollBar()->value();

      if ((row > activeRow) && (activeRow != ActionListController::NoItemActive))
      {
         // an item above also has control bar space
         vpRectTop += m_controllerHeigth;
      }

      // active element also has 'play control bar'
      if (row == activeRow)
      {
         height += m_controllerHeigth;
      }

      // rectangle is defined by row and vertical scrolling value
      visRect =  QRect(0, vpRectTop, viewport()->width(), height);
   }
   else
   {
      visRect = QRect();
   }

   return visRect;
}

/**
 * @brief move scrollbars in such way that a given element is visible.
 *    No 'hint' is supported: if element is above visible region, it will
 *    appare at top; if below, will appare at bottom.
 * @param index - entry of the view
 */
void ActionListView::scrollTo(const QModelIndex &index, ScrollHint /*hint*/)
{
   QRect area = viewport()->rect();                    // coordinate of visible area
   QRect rect = visualRectFromRow( index.row() );    // coordinate in widget

   if( (area != QRect()) && (rect != QRect()) )
   {
      // horizontal adjust
      if (rect.left() < area.left())
      {
         // too much on the left
         horizontalScrollBar()->setValue( horizontalScrollBar()->value() +\
                                          rect.left() - area.left());
      }
      else if (rect.right() > area.right())
      {
         // too much on the right
         horizontalScrollBar()->setValue( horizontalScrollBar()->value() +\
                                          qMin( rect.right() - area.right(),
                                                rect.left() - area.left()) );
      }

      if (rect.top() < area.top())
      {
         // too much high
         verticalScrollBar()->setValue( verticalScrollBar()->value() +\
                                        rect.top() - area.top());
      }
      else if (rect.bottom() > area.bottom())
      {
         // too much low
         verticalScrollBar()->setValue( verticalScrollBar()->value() +\
                                        qMin( rect.bottom() - area.bottom(),
                                              rect.top() - area.top()) );
      }

      viewport()->update();
   }
}

/**
 * @brief this moves the entire viewport by \p dx in horizontal
 *        and \p dy in vertical, without internal scrolling
 */
void ActionListView::scrollContentsBy(int dx, int dy)
{
   viewport()->scroll(dx, dy);
   viewport()->update();
}

/**
 * @brief set geometries as invalid, so they are updated
 */
void ActionListView::invalidateGeometries()
{
   updateGeometries();
   viewport()->update();
}

/**
 * @brief setter method for controller. This function has the
 *    post condition that that m_controller is not nullptr
 *
 * @param controller - if nullptr, a default widget is assigned
 */
void ActionListView::setActionBar(QWidget *actionBar)
{
   if (actionBar != nullptr)
   {
      m_actionBar = actionBar;
      m_actionBar->setParent( this);

      m_controllerHeigth = actionBar->height();
   }
   else
   {
      m_actionBar = fakeController;
   }
}

/**
 * @brief this is called when widget is resized
 */
void ActionListView::updateGeometries()
{
   QAbstractItemView::updateEditorGeometries();

   // update vertical scrollbar
   if( model() != nullptr)
   {
      verticalScrollBar()->setSingleStep( m_itemHeight );

      verticalScrollBar()->setPageStep(viewport()->height());

      int vLimit = (model()->rowCount() * m_itemHeight ) + m_controllerHeigth - viewport()->height();

      verticalScrollBar()->setRange( 0, qMax( 0, vLimit) );
   }
}

/**
 * @param point - in viewport coordinates
 * @return the model index of the item that occupies a screen point \p point
 */
QModelIndex ActionListView::indexAt(const QPoint &point) const
{
   QModelIndex ret_idx;

   // get the model row from vertical position
   int row = rowAt( point.y(), verticalScrollBar()->value());

   // in case no model is associated to view
   if( model() == nullptr)
   {
      ret_idx = QModelIndex();
   }
   else
   {
      if( (row >= model()->rowCount()) ||
          (point.x() < 0) ||
          (point.x() > viewport()->width()) )
      {
         // point is below the whole list of items
         ret_idx = QModelIndex();
      }
      else
      {
         // valid row
         ret_idx = model()->index( row, 0);
      }
   }

   return ret_idx;
}

/**
 * @brief calculate which row a given point belongs to.
 *    Only y coordinate is passed
 */
int ActionListView::rowAt( int yViewport, int verticalScroll) const
{
   int row;
   int activeRow = m_actionController.activeItemRow();

   /* point y in widget coordinates */
   int yWidget = yViewport + verticalScroll;

   /* top y of selected item */
   int yTopActive = activeRow * m_itemHeight;

   /* top y of item next to active one */
   int yNextToActive = yTopActive + (m_itemHeight + m_controllerHeigth);

   /* check which region the point is in */
   if ((yWidget < yTopActive) || (activeRow == ActionListController::NoItemActive))
   {
      /* point is before selection */
      row = yWidget / m_itemHeight;
   }
   else if( yWidget < yNextToActive)
   {
      /* point is in selected item */
      row = activeRow;
   }
   else
   {
      /* point is below selection */
      row = activeRow + 1 + ((yWidget - yNextToActive) / m_itemHeight);
   }

   return row;
}

/**
 * @param cursorAction - a movement by keyboard key
 * @return the model index of the 'next' item, where 'next' depends on keyboard move
 */
QModelIndex ActionListView::moveCursor(CursorAction cursorAction,
                                      Qt::KeyboardModifiers /*modifiers*/)
{
   QModelIndex current = currentIndex();
   int new_row = current.row();

   switch( cursorAction )
   {
   case MoveUp:
      new_row = isCurrentIndexValid( current ) ? (new_row - 1) : 0;
      break;

   case MoveDown:
      new_row = isCurrentIndexValid( current ) ? (new_row + 1) : 0;
      break;

   case MoveLeft:
   case MoveRight:
   default:
      break;
   }

   return model()->index( new_row, 0);
}

bool ActionListView::isCurrentIndexValid( const QModelIndex &index )
{
   return ( (index.row() >= 0) && (index.row() < model()->rowCount()) );
}

/**
 * @return the horizontal offset of the viewport, (horizontal scroll value)
 */
int ActionListView::horizontalOffset() const
{
   return horizontalScrollBar()->value();
}

/**
 * @return the vertical offset of the viewport, (vertical scroll value)
 */
int ActionListView::verticalOffset() const
{
   return verticalScrollBar()->value();
}

/** @brief return false; hidden elements are not supported */
bool ActionListView::isIndexHidden(const QModelIndex &/*index*/) const
{
   return false;
}

/**
 * @brief applies the selection flags to the items that intersect
 *        with area in \p rect
 * @param rect - a rectangle in widget coordinates
 * @param command - see ::QItemSelectionModel::SelectionFlag
 */
void ActionListView::setSelection(const QRect &rect,
                                 QItemSelectionModel::SelectionFlags command)
{
   // widget system is used
   QModelIndexList indexes;

   indexes = pickSelectedItems( rect.normalized() );
   markSelectedItems( &indexes, command );

   viewport()->update();
}

QModelIndexList ActionListView::pickSelectedItems( const QRect &selection_rect )
{
   QModelIndexList indexes;

   for( int i = 0; i < model()->rowCount(); i++)
   {
      QRect rVisual;
      int item_top, item_bottom;

      rVisual = visualRectFromRow( i );
      item_top = rVisual.top();
      item_bottom = rVisual.bottom();

      if( (item_bottom >= selection_rect.top()) &&
          (item_top <= selection_rect.bottom())
          )
      {
         indexes.append( model()->index( i, 0) );
      }
   }

   return indexes;
}

void ActionListView::markSelectedItems( QModelIndexList *selected_items,
                                         QItemSelectionModel::SelectionFlags command)
{
   if( selected_items->count() > 0 )
   {
      /* scan for first and last index */
      int item_first = selected_items->at(0).row();
      int item_last  = selected_items->last().row();

      for( int i = 0; i < selected_items->count(); i++)
      {
         item_first = qMin( item_first, selected_items->at(i).row() );
         item_last  = qMax( item_first, selected_items->at(i).row() );
      }

      QItemSelection selection( model()->index(item_first, 0, rootIndex()),
                                model()->index(item_last, 0, rootIndex())  );
      selectionModel()->select(selection, command);
   }
   else
   {
      QModelIndex noIndex;
      QItemSelection selection(noIndex, noIndex);
      selectionModel()->select(selection, command);
   }
}

/** to be called when view is updated, to reposition the
 * controller widget.
 * @par area - space assigned for the active item.
 *    The controller will use the lower part of this space
 */
void ActionListView::setControllerGeometry(const QRect &activeItemArea)
{
   /* calculate new controller geometry, (note the margin) */
   QRect newArea(CONTROLLER_HOR_OFFSET, activeItemArea.y() + CONTROLLER_VERT_OFFSET,
                 viewport()->width() - (2*CONTROLLER_HOR_OFFSET),
                 qMin(m_controllerHeigth, activeItemArea.height()));

   /* update playbar geometry only if needed */
   if( newArea != m_actionBar->geometry() )
   {
      m_actionBar->setGeometry( newArea);
   }
}


void ActionListView::shiftFocusedUp()
{
   m_actionController.shiftItemUp( selectionModel()->currentIndex().row() );
}

void ActionListView::shiftFocusedDown()
{
   m_actionController.shiftItemDown( selectionModel()->currentIndex().row());
}

void ActionListView::displayError( const QString &error)
{
   QMessageBox::warning( this, tr("Action error:"), error );
}

void ActionListView::onPanelFontSizeChanged(int ptSize)
{
   m_painter->onPanelFontSizeChanged( ptSize);
}

void ActionListView::setEditMode( bool editMode)
{
   m_editMode = editMode;

   m_activateSelectedAction->setEnabled( true);
   m_activateAndPlaySelectedAction->setEnabled( true);
   m_deleteSelectionAction->setEnabled( true);
   m_hidePlaybarAction->setEnabled( true);

   if (editMode == true)
   {
      m_insertIndex = INVALID_ITEM;
   }
}


/**
 * @param selection - info on selected items of the model
 * @return the region from the viewport of the items in the given selection
 */
QRegion ActionListView::visualRegionForSelection(const QItemSelection &selection) const
{
   QRegion region;
   QItemSelectionRange range;

   for( int i = 0; i < selection.count(); i++)
   {
      // merge visual rects for all selected items
      range = selection.at(i);
      for( int row = range.top(); row < range.bottom(); row++)
      {
         region += visualRectFromRow( row );
      }
   }

   return region;
}

/**
 * @brief this function calls the paint item method for all items.
 *    Subclasses of the painter must define how to paint each method.
 */
void ActionListView::paintEvent(QPaintEvent * /*e*/)
{
   T_ASSERT( model() != nullptr );

   int row_num = model()->rowCount();

   QPainter painter(viewport());
   m_painter->setPainter( &painter, this );

   // calculate first and last visible item
   int vscroll = verticalScrollBar()->value();
   int row_start = qMax((vscroll / m_itemHeight) - 1, 0);
   int row_max  = ( (vscroll + viewport()->height()) / m_itemHeight );

   // draw all visible items
   for (int i = row_start; (i <= row_max) && (i < row_num); i++)
   {
      paintListItem(i);
   }

   /* now reposition controller */
   QRect newControllerGeometry = visualRectFromRow( m_actionController.activeItemRow());
   setControllerGeometry( newControllerGeometry );

   /* in case, highlight insertion index */
   if (m_insertIndex > INVALID_ITEM)
   {
      QRect insertionRect = visualRectFromRow( m_insertIndex);

      painter.setPen( Qt::green);
      painter.drawRect( insertionRect.left(), insertionRect.top(),
                        insertionRect.width(), 3);
   }
}

void ActionListView::paintListItem( int position )
{
   QModelIndex index = model()->index( position, 0);

   QVariant itemData = index.data(modelViewRules::Binary);
   T_ASSERT( !itemData.isNull() );
   QRect item_rect = visualRectFromRow( position );

   if( position == m_actionController.activeItemRow() )
   {
      m_painter->paintActiveItem( item_rect, itemData, position );
   }
   else if( selectionModel()->isSelected(index) )
   {
      m_painter->paintSelectedItem( item_rect, itemData, position );
   }
   else
   {
      m_painter->paintRegularItem( item_rect, itemData, position );
   }
}

/**
 * @brief callback when an item is double-clicked
 * @param idx - double clicked element
 */
void ActionListView::onDoubleClickEvent( QModelIndex & idx )
{
   m_actionController.activateRequest( idx.row());
}

/**
 * to be called when an item has been shifted to \p newPosition
 */
void ActionListView::onShifted(int newPosition)
{
   QModelIndex index = model()->index( newPosition, 0);
   selectionModel()->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect );
}

/**
 * remove active item from model.
 * NOTE If other items are selected, they are deselected.
 */
void ActionListView::deleteActiveItem()
{
   QModelIndex activeIndex = model()->index( m_actionController.activeItemRow(), 0);
   selectionModel()->select( activeIndex, QItemSelectionModel::ClearAndSelect);

   deleteSelection();
}

void ActionListView::deActivate()
{
   m_actionController.activateRequest( QModelIndex(), false);
}

void ActionListView::onHidePlaybarRequested()
{
   m_actionController.activateRequest( -1);
}

void ActionListView::requestToDeleteSelection()
{
   deleteSelection();
}

/**
 * @brief remove selected items from model
 */
void ActionListView::deleteSelection()
{
   setUpdatesEnabled(false);
   QModelIndexList indexes = selectionModel()->selectedIndexes();

   m_actionController.removeItemsRequest( indexes);

   invalidateGeometries();
   setUpdatesEnabled(true);
}

void ActionListView::onActivateSelectedRequest()
{
   m_actionController.activateRequest( selectionModel()->currentIndex());
}

void ActionListView::onActivateAndPlaySelectedRequest()
{
   m_actionController.activateRequest( selectionModel()->currentIndex(),
                                       ActionListController::PLAY_IMMEDIATE);
}

void ActionListView::mousePressEvent(QMouseEvent * event)
{
   QAbstractItemView::mousePressEvent( event);

   /* PATCH: it seems that a single click outside list region
    * does not immediately deselect all items. So the item is still
    * drawn as selected but actually it is not */
   QPoint pos = event->pos();
   QPersistentModelIndex index = indexAt(pos);

   if ( ( ! index.isValid()) && (event->modifiers() == 0))
   {
      selectionModel()->select( QModelIndex(), QItemSelectionModel::Clear);
      repaint();
   }
}

void ActionListView::mouseDoubleClickEvent( QMouseEvent *event)
{
   //QAbstractItemView::mouseDoubleClickEvent( event);
   QModelIndex index = indexAt( event->pos());

   if (index.isValid() && (event->modifiers() == 0))
   {
      onDoubleClickEvent( index);
   }
   else
   {
      m_actionController.activateRequest( QModelIndex(), false);
   }
}


void ActionListView::keyPressEvent(QKeyEvent *event)
{
   QAbstractItemView::keyPressEvent( event);

   if (m_editMode)
   {
      if (event->matches( QKeySequence::Redo))
      {
         event->accept();
         emit redoRequest();
      }
      else if (event->matches( QKeySequence::Undo))
      {
         event->accept();
         emit undoRequest();
      }
   }
}

