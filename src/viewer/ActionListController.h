#ifndef ACTIONLISTCONTROLLER_H
#define ACTIONLISTCONTROLLER_H

#include <QObject>

class ExtAbstractListModel;
class QModelIndex;

/**
 * This class holds the info of the active item of an \a ActionListView,
 * (i.e. the item that has playbar on).
 * When an item is moved or deleted, index of active item is changed accordingly.
 */
class ActionListController : public QObject
{
   Q_OBJECT
public:
   explicit ActionListController(ExtAbstractListModel & model, QObject *parent = nullptr);

   static const int NoItemActive;

   /** flag for playing an item as soon as it's activated */
   static const bool PLAY_IMMEDIATE;

   int activeItemRow() const {
      return m_activeItemRow;
   }

   void shiftItemUp( int itemRow);
   void shiftItemDown( int itemRow);
   void removeItemsRequest(QList<QModelIndex> &indexList);

   /** move all items in \p indexList just before the item that,
    * before this operation, has index \p insertPosition */
   void moveItemsRequest(QList<QModelIndex> & indexList, int insertPosition);

public slots:
   void activateRequest( const QModelIndex &item, bool playImmediate = false);
   void activateRequest(int itemRow, bool playImmediate = false, bool dontStopFlag = false);

signals:
   void activeRowChanged( int, bool);
   void requestToPlay();
   void shiftedUp( int);
   void shiftedDown( int);

private:
   int m_activeItemRow;
   ExtAbstractListModel & m_model;

private slots:
   void onRowsRemoved(const QModelIndex &, int first, int last);
};

#endif // ACTIONLISTCONTROLLER_H
