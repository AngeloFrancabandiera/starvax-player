#include "DisconnectCommand.h"

#include "testableAssert.h"
#include "CommandReply_IF.h"
#include "NetworkInterface.h"


Server::DisconnectCommand::DisconnectCommand( CommandReply_IF & replySink) :
   m_replySink( replySink),
   m_networkProxy( nullptr)
{
}


bool Server::DisconnectCommand::execute( const QStringList & /*parameters*/)
{
   T_ASSERT (m_networkProxy != nullptr);

   /* say goodbye before closing connection */
   m_replySink.sendReplay( COMMAND_ACK, DISCONNECT, "See you!");
   m_networkProxy->closeClientConnection();

   return true;
}

