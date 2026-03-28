#ifndef CONTROLOWNCOMMAND_H
#define CONTROLOWNCOMMAND_H

#include "Command_IF.h"
class OpenWebNetEngine_IF;


namespace Server {

class CommandReply_IF;


class ControlOwnCommand : public Command_IF
{
public:
   ControlOwnCommand( CommandReply_IF & cmdReply,
                      OpenWebNetEngine_IF & ownEngine);

   virtual ~ControlOwnCommand() {}

   // Command_IF interface
public:
   virtual quint8 operativeCode() const {
      return CONTROL_OPEN_WEB_NET;
   }

   /**
    * @brief this command activates an Open Web Net command
    * @param parameters has two strings:
    *   - the first is 'where' of OWN command;
    *   - the second can be "ON" "OFF" or "DIMMER x",
    *   where x can be 1 to 10: 1 is off, 2-10 is 20% to 100%
    * @return always true
    */
   virtual bool execute(const QStringList & parameters);

private:
   CommandReply_IF & m_cmdReply;
   OpenWebNetEngine_IF & m_ownEngine;
};

}  // namespace Server

#endif // CONTROLOWNCOMMAND_H
