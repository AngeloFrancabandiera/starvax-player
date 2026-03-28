#include "ControlOwnCommand.h"
#include <QStringList>
#include <QRegularExpression>

#include "CommandReply_IF.h"
#include "OpenWebNetEngine_IF.h"

namespace  {

QRegularExpression DimmerRegExp("DIMMER (\\d+)");

}  // namespace


namespace Server {


ControlOwnCommand::ControlOwnCommand( CommandReply_IF & cmdReply,
                                      OpenWebNetEngine_IF & ownEngine) :
   m_cmdReply( cmdReply),
   m_ownEngine( ownEngine)
{
}

bool ControlOwnCommand::execute( const QStringList & parameters)
{
   bool executed = false;

   if (parameters.size() >= 2)
   {
      QString whereParam = parameters.at(0);
      QString levelParam = parameters.at(1);

      if (levelParam == "ON")
      {
         m_ownEngine.turnOn( whereParam);
      }
      else if (levelParam == "OFF")
      {
         m_ownEngine.turnOff( whereParam);
      }
      else
      {
         // check for dimmer command
         bool valid = false;

         QRegularExpressionMatch match = DimmerRegExp.match(levelParam);

         if (match.hasMatch())
         {
            int level = match.captured(1).toInt();

            if ((level >= 1) && (level <= 10))
            {
               valid = true;
               m_ownEngine.dimmer( level, whereParam);

               m_cmdReply.sendReplay( Server::COMMAND_ACK, CONTROL_OPEN_WEB_NET, "done");
               executed = true;
            }
         }

         if ( ! valid)
         {
            m_cmdReply.sendReplay( Server::COMMAND_INVALID_ARGUMENT, CONTROL_OPEN_WEB_NET,
                                   "level can be 'ON', 'OFF' or 1-10");
         }
      }


   }
   else
   {
      m_cmdReply.sendReplay( Server::COMMAND_NOT_ENOUGH_ARGS, CONTROL_OPEN_WEB_NET,
                             "Two arguments are required: 'where' and level");
   }

   return executed;
}

}  // namespace Server

