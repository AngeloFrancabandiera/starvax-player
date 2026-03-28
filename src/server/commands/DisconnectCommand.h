#ifndef DISCONNECTCOMMAND_H
#define DISCONNECTCOMMAND_H

#include "Command_IF.h"

namespace Server {

class CommandReply_IF;
class NetworkInterface;

/**
 * @brief The DisconnectCommand termiantes the connection of a client
 *   with the server.
 */
class DisconnectCommand : public Command_IF
{
public:
   DisconnectCommand( CommandReply_IF & replySink);
   ~DisconnectCommand() override {}

   /**
    * @brief linkNetworkInterface
    * @param networkProxy
    */
   void linkNetworkProxy( NetworkInterface * networkProxy) {
      m_networkProxy = networkProxy;
   }

   // Command_IF interface
public:
   quint8 operativeCode() const override {
      return DISCONNECT;
   }

   bool execute(const QStringList & parameters) override;

private:
   CommandReply_IF & m_replySink;
   NetworkInterface * m_networkProxy;
};

}  // namespace Server

#endif // DISCONNECTCOMMAND_H
