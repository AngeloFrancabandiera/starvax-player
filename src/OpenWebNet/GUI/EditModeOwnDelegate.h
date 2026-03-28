#ifndef EDITMODEOWNDELEGATE_H
#define EDITMODEOWNDELEGATE_H

#include <QStyledItemDelegate>


class EditModeOwnDelegate : public QStyledItemDelegate
{
   Q_OBJECT
public:
   EditModeOwnDelegate( QObject * parent = nullptr);

   // QAbstractItemDelegate interface
public:
   virtual QWidget *createEditor( QWidget * parent, const QStyleOptionViewItem & option,
                                  const QModelIndex & index) const;

   virtual void setModelData( QWidget * editor, QAbstractItemModel * model,
                              const QModelIndex & index) const;

   virtual void setEditorData(QWidget * editor, const QModelIndex & index) const;
};

#endif // EDITMODEOWNDELEGATE_H
