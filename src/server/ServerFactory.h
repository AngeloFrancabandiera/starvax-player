#ifndef SERVERFACTORY_H
#define SERVERFACTORY_H

#include <QList>

class ApplicationSettings;
class MediaAutomation;
class MediaListModel;
class LightPresetModel;
class SequenceModel;
class OwnModel;
class OpenWebNetEngine_IF;
class IF_MediaEngineInterface;
class IF_LightEngineInterface;
class SequenceRunner;
class ShowFileInfo;

class QAction;
class StatusDisplay;

namespace Server {
class Command_IF;
class CommandReply_IF;
class NetworkInterface;
class ServerEngine;


class Factory
{
public:
   Factory();

   QList<QAction *> buildActions();

   Server::CommandReply_IF * buildCommandReply();

   Server::ServerEngine * buildEngine( QAction * startServerAction,
                                       QAction * stopServerAction,
                                       CommandReply_IF & cmdReply,
                                       StatusDisplay & statusDisplay);

   Server::NetworkInterface * buildNetworkInterface( Server::ServerEngine & engine,
                                                     Server::CommandReply_IF & cmdReply );

   QList<Server::Command_IF *> buildLoginCommands( Server::CommandReply_IF & cmdReply,
                                                   ApplicationSettings & appSettings,
                                                   Server::ServerEngine & serverEngine,
                                                   Server::NetworkInterface & networkProxy);

   QList<Server::Command_IF *> buildShowCommands( Server::CommandReply_IF & cmdReply,
                                                  ShowFileInfo & showInfo);

   QList<Server::Command_IF *> buildPlaylistCommands( Server::CommandReply_IF & cmdReply,
                                                      MediaAutomation & avAutomation_A,
                                                      MediaAutomation & avAutomation_B,
                                                      MediaListModel & mediaList_A,
                                                      MediaListModel & mediaList_B,
                                                      IF_MediaEngineInterface & mediaEngine_A,
                                                      IF_MediaEngineInterface & mediaEngine_B);

   QList<Server::Command_IF *> buildLightsetCommands( Server::CommandReply_IF & cmdReply,
                                                      LightPresetModel & lightModel,
                                                      IF_LightEngineInterface & lightEngine);

   QList<Server::Command_IF *> buildOwnCommands( Server::CommandReply_IF & cmdReply,
                                                 OwnModel & ownModel,
                                                 OpenWebNetEngine_IF &ownEngine);
};

} // namespace Server

#endif // SERVERFACTORY_H
