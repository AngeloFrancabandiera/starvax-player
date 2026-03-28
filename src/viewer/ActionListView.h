#ifndef ACTIONLISTVIEW_H
#define ACTIONLISTVIEW_H

#include <QAbstractItemView>
#include "ActionListPainter.h"

class QPushButton;
class ActionListController;
class StatusDisplay;


class ActionListView : public QAbstractItemView
{
   Q_OBJECT
public:
   explicit ActionListView( ActionListController & actionController,
                            ActionListPainter *painter,
                            StatusDisplay & msgDisplay, QWidget *parent = nullptr);
   ~ActionListView() override;

   /* invalid index for active element */
   static const int INVALID_ITEM = -1;

public:
   QRect visualRect(const QModelIndex &index) const override;
   void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
   QModelIndex indexAt(const QPoint &point) const override;

   void setActionBar(QWidget *actionBar);

   /** this is used only in Message box dialogs */
   void setTitle( const QString & title) {
      m_title = title;
   }

   /** set position where an item is being inserted
    * (for example in drag-n-drop operation)
    * @param index is -1 when nothing is being inserted */
   void setInsertIndex( int  index) {
      m_insertIndex = index;
   }

signals:
   void redoRequest();
   void undoRequest();

public slots:
   void shiftFocusedUp();
   void shiftFocusedDown();
   void requestToDeleteSelection();
   void invalidateGeometries();
   void resetStyle();
   void displayError( const QString &);
   void onPanelFontSizeChanged( int ptSize);
   void deleteActiveItem();
   /** de-activate an eventual active item  */
   void deActivate();

   /**
    * @brief enter/exit edit mode.
    * @param editMode: when TRUE, GUI is allowed to change model
    */
   void setEditMode( bool editMode);

protected:  // pure virtual functions implementation
   QModelIndex moveCursor(CursorAction cursorAction,
                          Qt::KeyboardModifiers modifiers) override;

   int horizontalOffset() const override;
   int verticalOffset() const override;

   bool isIndexHidden(const QModelIndex &index) const override;

   void setSelection(const QRect &rect,
                     QItemSelectionModel::SelectionFlags command) override;
   QRegion visualRegionForSelection(const QItemSelection &selection) const override;

   void scrollContentsBy(int dx, int dy) override;

protected:
   void paintEvent(QPaintEvent *e) override;
   void mousePressEvent(QMouseEvent * event) override;
   void mouseDoubleClickEvent(QMouseEvent *event) override;
   void keyPressEvent(QKeyEvent *event) override;

   /** child classes use 'editMode' property in read-only */
   bool isEditMode() const {
      return m_editMode;
   }

protected slots:
   void updateGeometries() override;
   void onDoubleClickEvent(QModelIndex & idx);
   void onShifted(int newPosition);
   void onHidePlaybarRequested();

private:
   void paintListItem(int position );
   bool isCurrentIndexValid( const QModelIndex &index );
   QRect visualRectFromRow( int row ) const;
   QString format_display_text( const QStringList &display_data );
   QModelIndexList pickSelectedItems( const QRect &selection_rect );
   void markSelectedItems( QModelIndexList *selected_items,
                           QItemSelectionModel::SelectionFlags command);
   void setControllerGeometry(const QRect &activeItemArea);


   /* semi-constants reconfigurable by children */
protected:
   int m_itemHeight;
   int m_controllerHeigth;

private:
   ActionListController & m_actionController;
   ActionListPainter *m_painter;
   StatusDisplay & m_msgDisplay;
   bool m_editMode;
   QWidget *m_actionBar;
   QString m_title;
   int m_insertIndex;
   QAction * m_deleteSelectionAction;
   QAction * m_activateSelectedAction;
   QAction * m_activateAndPlaySelectedAction;
   QAction * m_hidePlaybarAction;


private:
   int rowAt(int yViewport, int verticalScroll) const;
   void deleteSelection();
private slots:
   void onActivateSelectedRequest();
   void onActivateAndPlaySelectedRequest();
};


#endif // ACTIONLISTVIEW_H
