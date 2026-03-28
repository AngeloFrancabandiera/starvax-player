#ifndef GETOWNLISTCOMMAND_H
#define GETOWNLISTCOMMAND_H

#include "Command_IF.h"
class ShowFileInfo;
class OwnModel;

namespace Server {

class CommandReply_IF;


class GetOwnListCommand : public Command_IF
{
public:
   GetOwnListCommand( CommandReply_IF & cmdReply,
                      OwnModel & ownModel);
   ~GetOwnListCommand() override {}

private:
   CommandReply_IF & m_replySink;
   OwnModel & m_ownModel;

   // Command_IF interface
public:
   quint8 operativeCode() const override{
      return GET_OPEN_WEB_NET_LIST;
   }

   /**
    * @brief this command return the list of rooms managed
    *    by Open Web Net pane
    * @param parameters is unused
    * @return always true
    */
   bool execute(const QStringList & parameters) override;
};

}  // namespace Server

#endif // GETOWNLISTCOMMAND_H
