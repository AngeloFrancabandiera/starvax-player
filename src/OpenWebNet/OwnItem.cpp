#include "OwnItem.h"

OwnItem::OwnItem():
   dimmerable(false)
{
}

OwnItem::OwnItem(QString aLabel, QString aWhere,
                 bool aDimmerable):
   label(aLabel),
   where(aWhere),
   dimmerable( aDimmerable)
{
}
