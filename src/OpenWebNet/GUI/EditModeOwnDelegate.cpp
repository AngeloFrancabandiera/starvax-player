#include "EditModeOwnDelegate.h"
#include "OwnModel.h"
#include "testableAssert.h"
#include "OwnItem.h"

#include <QCheckBox>

EditModeOwnDelegate::EditModeOwnDelegate(QObject * parent) :
   QStyledItemDelegate( parent)
{

}

QWidget *EditModeOwnDelegate::createEditor( QWidget * parent,
                                            const QStyleOptionViewItem & option,
                                            const QModelIndex & index) const
{
   QWidget * widget = nullptr;

   if (index.column() == OwnModel::DIMMERABLE_COLUMN)
   {
      widget = new QCheckBox(tr("check if dimmerable"), parent);
      widget->setStyleSheet("QCheckBox {background-color: black; color: white}");
   }
   else
   {
      /* revert to base class */
      widget = QStyledItemDelegate::createEditor( parent, option, index);
   }

   return widget;
}

void EditModeOwnDelegate::setModelData( QWidget * editor,
                                        QAbstractItemModel * model,
                                        const QModelIndex & index) const
{
   if (index.column() == OwnModel::DIMMERABLE_COLUMN)
   {
      QCheckBox * checkbox = dynamic_cast<QCheckBox *>(editor);
      T_ASSERT( checkbox);

      if (checkbox->isChecked())
      {
         model->setData( index, true);
      }
      else
      {
         model->setData( index, false);
      }
   }
   else
   {
      QStyledItemDelegate::setModelData( editor, model, index);
   }
}


void EditModeOwnDelegate::setEditorData( QWidget * editor,
                                         const QModelIndex & index) const
{
   if (index.column() == OwnModel::DIMMERABLE_COLUMN)
   {
      QCheckBox * checkbox = dynamic_cast<QCheckBox *>(editor);
      T_ASSERT( checkbox);

      const OwnItem * item = index.data( OwnModel::BINARY_ROLE).value<const OwnItem *>();
      T_ASSERT(item);

      checkbox->setChecked( item->dimmerable);
   }
   else
   {
      QStyledItemDelegate::setEditorData( editor, index);
   }
}
