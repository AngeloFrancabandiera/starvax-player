#ifndef SERVERENGINE_H
#define SERVERENGINE_H

#include <QMap>
#include "MessageEngine.h"


class TimeoutListener;
class ITimerService;

/* Not in namespace or class for signal/slot to work */
typedef enum {
   NOT_CONNECTED = 0,
   CONNECTING,
   CONNECTED

} ConnectionState;


namespace Server {

class Command_IF;
class CommandReply_IF;
class Status;

/**
 * @brief The ServerEngine class holds a set of commands that
 *   are triggered when a message from client is received with
 *   the operative code managed by the command.
 */
class ServerEngine : public MessageEngine
{
   Q_OBJECT
public:
   ServerEngine( Status & srvStatus, CommandReply_IF & replySink,
                 QObject * parent = nullptr);

   /**
    * @brief to be called by lower level interface when communication
    *   channel is up.
    * Connection is not yet established, i.e. user has not yet used lower
    * level channel to connect to server (only the "physical" channel is up).
    */
   void onConnectionActivated();

   /**
    * @brief to be called by lower level interface when communication
    *   channel is down.
    *   It will be necessary to set password again before gaining control.
    */
   void onConnectionClosed();

   /**
    * @brief to be called whenever a client establishes a connection.
    *   It is supposed that such connection is not yet granted and a timeout
    *   is armed to wait for credentials.
    */
   void onConnectionEstablished();

   /**
    * @brief to be called when message \a SET_CREDENTIALS is received with
    *   the ** correct ** password
    */
   void onPasswordGranted();

   /**
    * @brief add a command managed by the engine.
    * @param command must have a lifetime equal or larger than this class.
    */
   void addCommand( Command_IF * command);

   /**
    * @brief utility to call \a addCommand with many commands
    * @param commands
    */
   void addCommands( QList<Server::Command_IF *> & commands);

   /**
    * @brief to be called when a message from the network arrives
    * @param opCode identifies a command
    * @param params is a set of string extracted from message by null termination.
    * @return true if the message is executed correctly.
    */
   bool onMessageReceived(quint8 opCode, const QStringList & params) override;

   /**
    * @brief to be called by Network service when no TCP socet is
    *     listening on default port.
    */
   void onConnectionStopped();

private slots:
   void onCredentialsTimeout();

signals:
   /**
    * @brief emitted after a given time has elapsed since a client
    *   connected, without receiving from client valid credentials.
    */
   void grantTimeoutExpired();

private:
   Status & m_srvStatus;
   CommandReply_IF & m_replySink;
   QMap< quint8, Command_IF *> m_commandLookup;

   /** true when password has been granted */
   bool m_isConnectionGranted;
   TimeoutListener * m_timeoutListener;
   ITimerService * m_timerService;
};


}  // namespace Server


#endif // SERVERENGINE_H
