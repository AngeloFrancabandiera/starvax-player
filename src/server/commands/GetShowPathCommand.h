#ifndef GETSHOWPATHCOMMAND_H
#define GETSHOWPATHCOMMAND_H

#include "Command_IF.h"
class ShowFileInfo;

namespace Server {

class CommandReply_IF;


class GetShowPathCommand : public Command_IF
{
public:
   GetShowPathCommand( CommandReply_IF & cmdReply,
                       ShowFileInfo & showInfo );
   ~GetShowPathCommand() override {}

   // Command_IF interface
public:
   quint8 operativeCode() const override {
      return GET_SHOW_TITLE;
   }

   /**
    * @brief this command retrieves the full path of current show
    * @param parameters is void
    * @return always True
    */
   bool execute(const QStringList & parameters) override;

private:
   CommandReply_IF & m_replySink;
   ShowFileInfo & m_showInfo;
};

}  // namespace Server

#endif // GETSHOWPATHCOMMAND_H
