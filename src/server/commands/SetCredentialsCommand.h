#ifndef SETCREDENTIALSCOMMAND_H
#define SETCREDENTIALSCOMMAND_H

#include "Command_IF.h"

#include <QString>
class ApplicationSettings;


namespace Server {

class CommandReply_IF;
class ServerEngine;
class NetworkInterface;


/**
 * @brief The SetCredentialsCommand passes the password
 *   that allows the client to send further commands that will
 *   be executed.
 */
class SetCredentialsCommand : public Command_IF
{
public:
   SetCredentialsCommand( const ApplicationSettings & settings,
                          CommandReply_IF & replySink);
   ~SetCredentialsCommand() override {}

   /**
    * @brief link the \a ServerEngine instance that will be notified when
    *   correct password is given.
    * @param engine will be called when  correct password is given.
    */
   void linkReplyEngine( ServerEngine * engine) {
      m_engine = engine;
   }

   /**
    * @brief link the \a NetworkProxy that may be used to disconnect
    *   client in case of wrong password.
    * @param networkProxy
    */
   void linkNetworkProxy( NetworkInterface * networkProxy) {
      m_networkProxy = networkProxy;
   }

   // Command_IF interface
public:
   quint8 operativeCode() const override {
      return SET_CREDENTIALS;
   }

   bool isAllowedWithoutGrant() const override{
      return true;
   }

   bool execute(const QStringList & parameters) override;

private:
   const ApplicationSettings & m_settings;
   CommandReply_IF & m_replySink;

   ServerEngine * m_engine;
   NetworkInterface * m_networkProxy;
};

}  // namespace Server

#endif // SETCREDENTIALSCOMMAND_H
