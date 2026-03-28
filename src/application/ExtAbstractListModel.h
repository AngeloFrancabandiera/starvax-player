#ifndef EXTABSTRACTLISTMODEL_H
#define EXTABSTRACTLISTMODEL_H

#include <QAbstractListModel>


class ExtAbstractListModel : public QAbstractListModel
{
   Q_OBJECT
public:
   ExtAbstractListModel( QObject *parent=nullptr) :
      QAbstractListModel(parent)
   {}


   /** undo last operation on internal model */
   virtual void undo() = 0;
   /** redo last operation on internal model */
   virtual void redo() = 0;

   /** start a set of operation that will be undone together */
   virtual void beginUndoMacro( const QString & name) = 0;
   /** end a set of operation that will be undone together */
   virtual void endUndoMacro() = 0;

   /** move a set of items (not necessarily continuous) and store them
    *   continuously at a given index
    * @param selectedIndexes holds the indexes that have been selected
    * @param targetRow is the row at which the first item will be placed after re-arrangement
    */
   virtual void rearrangeItems( QList<QModelIndex> & selectedIndexes, int targetRow ) = 0;
};

/* compare two QModelIndex instances by row */
inline bool rowGreater( const QModelIndex & m1, const QModelIndex & m2)
{
   return m1.row() > m2.row();
}

#endif // EXTABSTRACTLISTMODEL_H

