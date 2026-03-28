#include "ServerFactory.h"
#include "testableAssert.h"
#include "CommandReply_IF.h"
#include "Action.h"
#include "ApplicationIcon.h"

#include "MessageParser.h"
#include "NetworkInterfaceQt.h"
#include "NetworkCommandReply.h"
#include "ServerStatus.h"
#include "ServerEngine.h"

#include "SetCredentialsCommand.h"
#include "DisconnectCommand.h"
#include "GetShowPathCommand.h"
#include "SetActiveMediaCommand.h"
#include "GetMediaListCommand.h"
#include "GetLightsetListCommand.h"
#include "PlayMediaCommand.h"
#include "PauseMediaCommand.h"
#include "StopMediaCommand.h"
#include "RewindMediaCommand.h"
#include "SelectLightTransitionCommand.h"
#include "GetOwnListCommand.h"
#include "ControlOwnCommand.h"

namespace Server {

Factory::Factory()
{

}

QList<QAction *> Factory::buildActions()
{
   Action * startServerAction = new Action( ApplicationIcon("serverStart.png"),
                                            QObject::tr("&Start server"), nullptr);
   startServerAction->bindShortcut( Qt::ALT | Qt::Key_S);
   startServerAction->setEnabled(true);
   startServerAction->setVisibleInModes( false, true);

   Action * stopServerAction = new Action( ApplicationIcon("serverStop.png"),
                                           QObject::tr("S&top server"), nullptr);
   stopServerAction->bindShortcut( QKeySequence(Qt::ALT | Qt::SHIFT | Qt::Key_S));
   stopServerAction->setEnabled(false);
   stopServerAction->setVisibleInModes( false, true);

   return QList<QAction *>() << startServerAction << stopServerAction;
}

CommandReply_IF * Factory::buildCommandReply()
{
   return new NetworkCommandReply();
}

Server::ServerEngine *Factory::buildEngine( QAction * startServerAction,
                                            QAction * stopServerAction,
                                            CommandReply_IF & cmdReply,
                                            StatusDisplay & statusDisplay)
{
   T_ASSERT( startServerAction);
   T_ASSERT( stopServerAction);

   Server::Status * serverStatus = new Server::Status( *startServerAction,
                                                       *stopServerAction, statusDisplay);

   Server::ServerEngine * serverEngine =
         new Server::ServerEngine( *serverStatus, cmdReply, nullptr);

   return serverEngine;
}

Server::NetworkInterface * Factory::buildNetworkInterface( Server::ServerEngine & engine,
                                                           CommandReply_IF & cmdReply )
{
   Server::NetworkCommandReply * netReply =
         dynamic_cast<Server::NetworkCommandReply *>( & cmdReply);

   /* normally runtime type check is a bad idea, but in this case we rely that the
    * 'CommandReply_IF' instance has been created by the same factory that is
    * creating network interface. So we en-force that the reply must be a network reply
    */
   T_ASSERT( netReply);

   Server::NetworkInterfaceQt * networkProxy =
         new Server::NetworkInterfaceQt( engine, *netReply, & engine);

   Server::MessageParser * messageParser = new Server::MessageParser();
   networkProxy->setParser( messageParser);
   messageParser->addEngine( & engine);

   return networkProxy;
}

QList<Server::Command_IF *> Factory::buildLoginCommands( Server::CommandReply_IF & cmdReply,
                                                         ApplicationSettings & appSettings,
                                                         Server::ServerEngine & serverEngine,
                                                         Server::NetworkInterface & networkProxy)
{
   QList<Server::Command_IF *> commands;

   Server::SetCredentialsCommand * setCredentialsCommand =
         new Server::SetCredentialsCommand( appSettings, cmdReply);
   Server::DisconnectCommand * disconnectCommand =
         new Server::DisconnectCommand( cmdReply);

   commands << setCredentialsCommand << disconnectCommand;

   setCredentialsCommand->linkReplyEngine( & serverEngine);
   disconnectCommand->linkNetworkProxy( & networkProxy);
   setCredentialsCommand->linkNetworkProxy( & networkProxy);

   return commands;
}

QList<Command_IF *> Factory::buildShowCommands( CommandReply_IF & cmdReply,
                                                ShowFileInfo & showInfo)
{
   return QList<Server::Command_IF *>() << new GetShowPathCommand( cmdReply, showInfo);
}

QList<Server::Command_IF *> Factory::buildPlaylistCommands(
      Server::CommandReply_IF & cmdReply,
      MediaAutomation & avAutomation_A,
      MediaAutomation & avAutomation_B,
      MediaListModel & mediaList_A,
      MediaListModel & mediaList_B,
      IF_MediaEngineInterface & mediaEngine_A,
      IF_MediaEngineInterface & mediaEngine_B)
{
   QList<Server::Command_IF *> commands;

   commands << new Server::SetActiveTrackCommand( avAutomation_A, avAutomation_B,
                                                  mediaList_A, mediaList_B, cmdReply)
            << new Server::GetMediaListCommand( mediaList_A, mediaList_B, cmdReply)
            << new Server::PlayMediaCommand( mediaEngine_A, mediaEngine_B, cmdReply)
            << new Server::PauseMediaCommand( mediaEngine_A, mediaEngine_B, cmdReply)
            << new Server::StopMediaCommand( mediaEngine_A, mediaEngine_B, cmdReply)
            << new Server::RewindMediaCommand( mediaEngine_A, mediaEngine_B, cmdReply);

   return commands;
}

QList<Server::Command_IF *> Factory::buildLightsetCommands(
      Server::CommandReply_IF & cmdReply,
      LightPresetModel & lightModel,
      IF_LightEngineInterface & lightEngine)
{
   QList<Server::Command_IF *> commands;

   // TBD missing "fire current transition"
   commands << new Server::GetLightsetListCommand( lightModel, cmdReply)
            << new Server::SelectLightTransitionCommand( lightEngine, lightModel, cmdReply);

   return commands;
}

QList<Command_IF *> Factory::buildOwnCommands( CommandReply_IF & cmdReply,
                                               OwnModel & ownModel,
                                               OpenWebNetEngine_IF & ownEngine)
{
   QList< Server::Command_IF * > commands =
         QList< Server::Command_IF * >()
         << new GetOwnListCommand( cmdReply, ownModel)
         << new ControlOwnCommand( cmdReply, ownEngine);

   return commands;
}



} // namespace Server
