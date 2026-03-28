#include "OperativeOwnDelegate.h"
#include "OpenWebNetEngine_IF.h"
#include "OwnActuatorWidget.h"
#include "OwnModel.h"
#include "testableAssert.h"
#include "OwnItem.h"


OperativeOwnDelegate::OperativeOwnDelegate(OpenWebNetEngine_IF &ownEngine,
                                            QObject * parent) :
   QStyledItemDelegate(parent),
   m_ownEngine(ownEngine)
{
}


QWidget *OperativeOwnDelegate::createEditor( QWidget * parent,
                                             const QStyleOptionViewItem & /*option*/,
                                             const QModelIndex & index) const
{
   const OwnItem * item = index.data( OwnModel::BINARY_ROLE).value<const OwnItem *>();
   T_ASSERT( item);

   // qt will delete this at the end of edit operation
   OwnActuatorWidget * widget = new OwnActuatorWidget( m_ownEngine, parent);
   widget->attachItem( *item);

   return widget;
}



void OperativeOwnDelegate::setModelData( QWidget * /*editor*/,
                                         QAbstractItemModel * /*model*/,
                                         const QModelIndex & /*index*/) const
{
   /* nothing to do because model was not really edited */
}
