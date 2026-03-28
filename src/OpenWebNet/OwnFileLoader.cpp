#include "OwnFileLoader.h"
#include <QMessageBox>

#include "OwnModel.h"
#include "OwnFileParser.h"
#include "testableAssert.h"


OwnFileLoader::OwnFileLoader(OwnModel & ownModel, QObject *parent) :
   QObject(parent),
   m_ownModel( ownModel)
{
}

void OwnFileLoader::loadConfigurationFile(const QString & fullPath)
{
   OwnFileParser parser;
   parser.parseFile( fullPath);

   const QStringList & errors = parser.errors();

   if (errors.size() > 0)
   {
      QMessageBox::information( nullptr, tr("Open Web Net configuration"),
                                tr("<p>Open Web Net configuration file has the following"
                                   " problems. If possible, we will make our best to load"
                                   " the configuration.</p><p>%1</p>").
                                arg(errors.join("<br>")));
   }

   const QList<const OwnItem *> & ownItems = parser.openWebNetItems();

   if (ownItems.size() > 0)
   {
      m_ownModel.clear();

      foreach (const OwnItem * item, ownItems)
      {
         T_ASSERT( item);

         /* note that 'item' will be deleted when this function returns,
          * because 'parser' is deleted */
         m_ownModel.addLightItem( item->label, item->where, item->dimmerable);
      }
   }

   emit configurationLoaded();
}
