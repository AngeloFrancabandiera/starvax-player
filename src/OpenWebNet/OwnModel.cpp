#include "OwnModel.h"
#include <OwnItem.h>
#include <QStringList>
#include <QMimeData>
#include <QDataStream>
#include <QIODevice>

#include "testableAssert.h"

namespace  {
QStringList HEADERS;

}  // namespace



OwnModel::OwnModel(QObject * parent) :
   QAbstractTableModel(parent)
{
   HEADERS << tr("Name")
           << tr("Where")
           << tr("Dimmer");
}

void OwnModel::addLightItem()
{
   m_ownItems << new OwnItem( "??", "00", false);
   emit layoutChanged();
}

void OwnModel::addLightItem( const QString & label,
                             const QString & where,
                             bool isDimmerable)
{
   m_ownItems << new OwnItem( label, where, isDimmerable);
   emit layoutChanged();
}

void OwnModel::clear()
{
   m_ownItems.clear();
}


int OwnModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_ownItems.size();
}

int OwnModel::columnCount(const QModelIndex & /*parent*/) const
{
   return HEADERS.size();
}


QVariant OwnModel::data(const QModelIndex & index, int role) const
{
   QVariant ret = QVariant();


   if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
   {
      T_ASSERT ((index.row() >= 0) && (index.row() < m_ownItems.size()));

      switch (index.column())
      {
      case OwnModel::LABEL_COLUMN:
         ret = m_ownItems.at(index.row())->label;
         break;

      case OwnModel::WHERE_COLUMN:
         ret = m_ownItems.at(index.row())->where;
         break;

      case OwnModel::DIMMERABLE_COLUMN:
         /* in edit role, this case is not called */
         ret = m_ownItems.at(index.row())->dimmerable ? tr("YES") : tr("NO");
         break;

      default:
         break;
      }

   }

   if (role == OwnModel::BINARY_ROLE)
   {
      T_ASSERT ((index.row() >= 0) && (index.row() < m_ownItems.size()));
      ret = QVariant::fromValue<const OwnItem *>(m_ownItems.at(index.row()));
   }

   return ret;
}

bool OwnModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
   bool res = false;


   if (role == Qt::EditRole)
   {
      T_ASSERT ((index.row() >= 0) && (index.row() < m_ownItems.size()));

      switch (index.column())
      {
      case OwnModel::LABEL_COLUMN:
         m_ownItems.at(index.row())->label = value.toString();
         res = true;
         break;

      case OwnModel::WHERE_COLUMN:
         m_ownItems.at(index.row())->where = value.toString();
         res = true;
         break;

      case OwnModel::DIMMERABLE_COLUMN:
         m_ownItems.at(index.row())->dimmerable =  value.toBool();
         res = true;
         break;

      default:
         T_ASSERT( false);
         break;
      }

      emit layoutChanged();

   }

   return res;
}

QVariant OwnModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   QVariant header;

   if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
   {
      if ((section >= 0) && (section < HEADERS.size()))
      {
         header = QVariant::fromValue<QString>( HEADERS.at(section));
      }
   }

   return header;
}


Qt::ItemFlags OwnModel::flags(const QModelIndex & /*index*/) const
{
   return Qt::ItemIsSelectable | Qt::ItemIsEnabled |
         Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
}



bool OwnModel::removeRow( int row, const QModelIndex & /*parent*/)
{
   T_ASSERT( row >= 0);
   T_ASSERT( row < m_ownItems.size());

   m_ownItems.removeAt( row);
   layoutChanged();

   return true;
}

QString OwnModel::tagForName( const QString & zoneName) const
{
   QString tag;

   foreach (OwnItem * item, m_ownItems)
   {
      if (item->label == zoneName)
      {
         tag = item->where;
      }
   }

   return tag;
}


QStringList OwnModel::mimeTypes() const
{
   QStringList types;
   types << "openWebNet-data";
   return types;
}

QMimeData *OwnModel::mimeData(const QModelIndexList &indexes) const
{
   /* it seems that this is automagically deleted at the end of drag operation */
   QMimeData * mimeData = new QMimeData();

   QByteArray encodedData;
   QDataStream stream( & encodedData, QIODevice::WriteOnly);

   foreach (const QModelIndex &index, indexes)
   {
      if (index.isValid() && (index.column() == 0))
      {
         stream << m_ownItems.at(index.row())->dimmerable;
         stream << m_ownItems.at(index.row())->where;
         stream << m_ownItems.at(index.row())->label;
      }
   }

   mimeData->setData( "openWebNet-data", encodedData);
   return mimeData;
}
