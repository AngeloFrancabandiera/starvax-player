#include "SetCredentialsCommand.h"
#include <QStringList>

#include "testableAssert.h"
#include "CommandReply_IF.h"
#include "ServerEngine.h"
#include "NetworkInterface.h"
#include "ApplicationSettings.h"

Server::
SetCredentialsCommand::SetCredentialsCommand( const ApplicationSettings & settings,
                                              CommandReply_IF & replySink) :
   m_settings( settings),
   m_replySink( replySink),
   m_engine( nullptr),
   m_networkProxy( nullptr)
{
}


bool Server::SetCredentialsCommand::execute(const QStringList & parameters)
{
   T_ASSERT (m_engine != nullptr);
   T_ASSERT (m_networkProxy != nullptr);

   bool executed = false;

   if (parameters.size() >= 1)
   {
      executed = true;

      if (parameters.first() == m_settings.remotePassword())
      {
         m_engine->onPasswordGranted();

         m_replySink.sendReplay( COMMAND_ACK, SET_CREDENTIALS,
                                 QString("you can now control the player"));
      }
      else
      {
         m_replySink.sendReplay( COMMAND_WRONG_PASSWORD, SET_CREDENTIALS,
                                 QString("password is not correct. You'll be disconnected."));
         m_networkProxy->closeClientConnection();
      }
   }
   else
   {
      m_replySink.sendReplay( COMMAND_NOT_ENOUGH_ARGS, SET_CREDENTIALS,
                              QString("please provide password"));
   }

   return executed;
}

