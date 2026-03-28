#include "GetShowPathCommand.h"
#include "ShowFileInfo.h"
#include "CommandReply_IF.h"

namespace Server {



GetShowPathCommand::GetShowPathCommand( CommandReply_IF & cmdReply,
                                        ShowFileInfo & showInfo ) :
   m_replySink( cmdReply),
   m_showInfo( showInfo)
{
}


bool GetShowPathCommand::execute(const QStringList & /*parameters*/)
{
   QString currentPath = m_showInfo.getCurrentShowFilePath();

   if (currentPath == QString())
   {
      currentPath = "NO SHOW IS OPEN";
   }
   m_replySink.sendReplay( Server::COMMAND_ACK, GET_SHOW_TITLE, currentPath);

   return true;
}

}  // namespace Server

