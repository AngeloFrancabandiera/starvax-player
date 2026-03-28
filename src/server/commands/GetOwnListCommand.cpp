#include "GetOwnListCommand.h"
#include "testableAssert.h"

#include "CommandReply_IF.h"
#include "OwnModel.h"
#include "OwnItem.h"

namespace Server {

GetOwnListCommand::GetOwnListCommand( CommandReply_IF & cmdReply,
                                      OwnModel & ownModel):
   m_replySink( cmdReply),
   m_ownModel( ownModel)

{
}

bool GetOwnListCommand::execute(const QStringList & /*parameters*/)
{
   QStringList items;

   int numberOfitems = m_ownModel.rowCount();

   const OwnItem * rowItem;

   for (int i=0; i < numberOfitems; i++)
   {
      rowItem = m_ownModel.index( i, OwnModel::LABEL_COLUMN).data( OwnModel::BINARY_ROLE)
                .value<const OwnItem *>();
      T_ASSERT( rowItem);

      items << QString("%1;%2;%3").
               arg(rowItem->label).
               arg(rowItem->where).
               arg(rowItem->dimmerable ? "DIMMER" : "NO_DIMMER");
   }

   m_replySink.sendReplay( Server::COMMAND_ACK,
                           GET_OPEN_WEB_NET_LIST, items);

   return true;
}

}  // namespace Server

