#include "ServerEngine.h"
#include "ServerStatus.h"
#include "Command_IF.h"
#include "CommandReply_IF.h"
#include "QtTimerService.h"

static const int GRANT_TIMEOUT_MS = 5000;


Server::ServerEngine::ServerEngine(Status & srvStatus, CommandReply_IF & replySink,
                                    QObject * parent) :
   MessageEngine(parent),
   m_srvStatus( srvStatus),
   m_replySink( replySink),
   m_isConnectionGranted( false),
   m_timeoutListener( new TimeoutListener(this)),
   m_timerService( new QtTimerService(this))
{
   connect( m_timeoutListener, SIGNAL(timeout()), this, SLOT(onCredentialsTimeout()));
}

void Server::ServerEngine::onConnectionActivated()
{
   m_srvStatus.onServerStarted();
}

void Server::ServerEngine::onConnectionStopped()
{
   m_srvStatus.onServerStopped();
}

void Server::ServerEngine::onCredentialsTimeout()
{
   m_srvStatus.onError( "Credentials timeout has expired");
   emit grantTimeoutExpired();
}

void Server::ServerEngine::onConnectionClosed()
{
   m_isConnectionGranted = false;
   m_srvStatus.onConnectionClosed();
}

void Server::ServerEngine::onConnectionEstablished()
{
   /* start timeout to wait for connection */
   m_timerService->triggerTimeout( GRANT_TIMEOUT_MS, m_timeoutListener);
   m_srvStatus.onConnecting();
}

void Server::ServerEngine::onPasswordGranted()
{
   /* stop timeout timer */
   m_timerService->stopTimer();

   m_isConnectionGranted = true;
   m_srvStatus.onConnectionUp();
}

void Server::ServerEngine::addCommand(Server::Command_IF * command)
{
   quint16 opCode = command->operativeCode();

   m_commandLookup.insert( opCode, command);
}

void Server::ServerEngine::addCommands(QList<Server::Command_IF *> & commands)
{
   foreach (Server::Command_IF * cmd, commands)
   {
      addCommand( cmd);
   }
}

bool Server::ServerEngine::onMessageReceived(quint8 opCode, const QStringList & params)
{
   bool executed = false;

   Command_IF * command = m_commandLookup.value( opCode, nullptr);

   if (command != nullptr)
   {
      if (m_isConnectionGranted || command->isAllowedWithoutGrant())
      {
         executed = command->execute( params);
      }
      else
      {
         m_replySink.sendReplay( COMMAND_NOT_GRANTED, opCode,
                                 QString("You must log in before sending commands."));
      }
   }
   else
   {
      m_replySink.sendReplay( COMMAND_UNSUPPORTED, opCode,
                              QString("command %1 not known.").arg((quint32)opCode));
   }

   return executed;
}



