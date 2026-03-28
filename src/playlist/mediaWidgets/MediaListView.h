#ifndef MEDIALISTVIEW_H
#define MEDIALISTVIEW_H

#include "ActionListView.h"
class MediaSourceFactoryInterface;
class ActionListController;

/**
 * @brief The MediaListView class specifies the drag-n-drop
 * behavior for \a ActionListView
 */
class MediaListView : public ActionListView
{
   Q_OBJECT
public:
   MediaListView( ActionListController & actionController,
                  ActionListPainter *painter,
                  StatusDisplay & msgDisplay, QWidget *parent = nullptr);

   ~MediaListView() override {}

   // QWidget interface
protected:
   void dragEnterEvent(QDragEnterEvent * event) override;
   void dragMoveEvent(QDragMoveEvent * event) override;
   void dragLeaveEvent(QDragLeaveEvent * event) override;
   void dropEvent(QDropEvent * event) override;
   // QAbstractItemView interface
protected:
   void startDrag(Qt::DropActions supportedActions) override;

private:
   ActionListController & m_actionListController;
   StatusDisplay & m_msgDisplay;
   int m_insertPosition;
   bool m_localDragFlag;

private:
   bool dragHasValidUrl(QDragEnterEvent* event);
   void addValidUrlsToModel( QDropEvent * event);
   void addDroppedMedia(QDropEvent* event);
   void selectDropOperation(QDropEvent* event);
   void updateInsertPosition();
   void handleDropOperation(QDropEvent* event);
};

#endif // MEDIALISTVIEW_H
