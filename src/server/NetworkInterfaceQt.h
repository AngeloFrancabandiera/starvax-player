#ifndef NETWORKINTERFACEQT_H
#define NETWORKINTERFACEQT_H

#include "NetworkInterface.h"
#include <QObject>
#include <QAbstractSocket>

class QTcpServer;
class QTcpSocket;


namespace Server {

class NetworkCommandReply;
class ServerEngine;



/**
 * @brief The NetworkInterfaceQt class is an implementation of
 * \a NetworkInterface class that uses a QTcpServer
 */
class NetworkInterfaceQt : public Server::NetworkInterface
{
   Q_OBJECT
public:
   NetworkInterfaceQt( Server::ServerEngine & serverEngine,
                       Server::NetworkCommandReply & commandReply,
                       QObject * parent = nullptr);
   ~NetworkInterfaceQt();

   // NetworkInterface interface
public:
   virtual void setParser( Server::MessageParser * parser);
   virtual void closeClientConnection();

public slots:
   virtual void startServer();
   virtual void stopServer();

private:
   Server::ServerEngine & m_serverEngine;
   Server::NetworkCommandReply & m_commandReply;
   QTcpServer * m_tcpServer;
   QTcpSocket * m_socket;
   Server::MessageParser * m_parser;

private slots:
   void onAcceptError( QAbstractSocket::SocketError);
   void onNewConnection();
   void onSocketDisconnected();
   void onSocketDataReady();
   void onGrantTimeoutExpired();
};

}  // namespace Server

#endif // NETWORKINTERFACEQT_H
