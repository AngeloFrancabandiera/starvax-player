#ifndef LIGHTSETLISTVIEW_H
#define LIGHTSETLISTVIEW_H

#include "ActionListView.h"



class LightsetListView : public ActionListView
{
   Q_OBJECT

public:
   LightsetListView( ActionListController & actionController,
                     ActionListPainter *painter,
                     StatusDisplay & msgDisplay, QWidget *parent = nullptr);

   ~ LightsetListView() override {}


   // QWidget interface
protected:
   void dragEnterEvent(QDragEnterEvent * event) override;
   void dragMoveEvent(QDragMoveEvent * event) override;
   void dragLeaveEvent(QDragLeaveEvent * event) override;
   void dropEvent(QDropEvent * event) override;

private:
   ActionListController & m_actionController;
   StatusDisplay & m_msgDisplay;
   int m_insertPosition;
   QString m_currentStylesheet;

private:
   void moveDroppedItems(QDropEvent * event);
};

#endif // LIGHTSETLISTVIEW_H
