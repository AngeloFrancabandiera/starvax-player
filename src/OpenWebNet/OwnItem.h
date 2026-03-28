#ifndef OWNITEM_H
#define OWNITEM_H

#include <QObject>
#include <QString>

/**
 * @brief The OwnItem class is a structured type
 * for an entry of an Open Web Net model
 */
class OwnItem: public QObject
{
   Q_OBJECT

public:
   OwnItem();
   OwnItem( QString aLabel,
            QString aWhere,
            bool aDimmerable);
   ~OwnItem() {}

   QString label;  /**< label printed in table */
   QString where;  /**< 'where' according to OWN protocol */
   bool dimmerable;  /**< false for ON/OFF; true for dimmered lights */
};

Q_DECLARE_METATYPE(const OwnItem *)

#endif // OWNITEM_H
