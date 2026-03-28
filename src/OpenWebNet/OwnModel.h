#ifndef OWNMODEL_H
#define OWNMODEL_H

#include <QAbstractTableModel>
#include <QList>

class OwnItem;

/**
 * @brief The OwnModel class holds all Open Web Net
 * configuration. Both edit and operational table read from
 * this model.
 */
class OwnModel : public QAbstractTableModel
{
   Q_OBJECT
public:
   OwnModel( QObject * parent = nullptr);

   static const int LABEL_COLUMN = 0;
   static const int WHERE_COLUMN = 1;
   static const int DIMMERABLE_COLUMN = 2;

   static const int BINARY_ROLE = Qt::UserRole + 1;

   /* retrieve OWN tag (eg. #1) for full zone name */
   QString tagForName( const QString & zoneName) const;

public slots:
   /**
    * @brief add a new Light item to model with empty values
    */
   void addLightItem();

   /**
    * @brief add a new Light item to model with given values
    * @param label
    * @param where
    * @param isDimmerable
    */
   void addLightItem( const QString & label, const QString & where, bool isDimmerable);

   /** remove all items from model */
   void clear();

   /**
    * @return a read-only reference of items.
    * This is mainly a shortcut to serialize data
    */
   const QList<OwnItem *> & openWebNetItems() const {
      return m_ownItems;
   }

   // QAbstractItemModel interface
public:
   int rowCount(const QModelIndex & parent = QModelIndex()) const override;
   int columnCount(const QModelIndex & parent) const override;
   QVariant data(const QModelIndex & index, int role) const override;
   bool setData(const QModelIndex & index, const QVariant & value, int role) override;
   QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
   Qt::ItemFlags flags(const QModelIndex & index) const override;
   bool removeRow(int row, const QModelIndex &);
   QStringList mimeTypes() const override;
   QMimeData *mimeData(const QModelIndexList &indexes) const override;

private:
   QList<OwnItem *> m_ownItems;

};

#endif // OWNMODEL_H
