#include "NetworkInterfaceQt.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include "testableAssert.h"
#include "MessageParser.h"
#include "NetworkCommandReply.h"
#include "ServerEngine.h"
#include "Command_IF.h"



namespace {
const quint16 SERVER_LISTEN_PORT = 12400u;
}

namespace Server {


NetworkInterfaceQt::NetworkInterfaceQt( Server::ServerEngine & serverEngine,
                                        Server::NetworkCommandReply & commandReply,
                                        QObject * parent) :
   NetworkInterface(parent),
   m_serverEngine( serverEngine),
   m_commandReply( commandReply),
   m_tcpServer( new QTcpServer(this)),
   m_socket( nullptr),
   m_parser( nullptr)
{
}

NetworkInterfaceQt::~NetworkInterfaceQt()
{
   if (m_socket != nullptr)
   {
      m_socket->disconnectFromHost();
      delete m_socket;
   }

   delete m_tcpServer;
}


void NetworkInterfaceQt::setParser( Server::MessageParser * parser)
{
   m_parser = parser;
}

void NetworkInterfaceQt::startServer()
{
   T_ASSERT (m_parser != nullptr);

   connect( m_tcpServer,SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(onAcceptError(QAbstractSocket::SocketError)) );
   connect( m_tcpServer, SIGNAL( newConnection()),
            this, SLOT(onNewConnection()) );
   connect( &m_serverEngine, SIGNAL(grantTimeoutExpired()),
            this, SLOT(onGrantTimeoutExpired()) );

   m_tcpServer->listen( QHostAddress::Any, SERVER_LISTEN_PORT);
   m_serverEngine.onConnectionActivated();

}


void NetworkInterfaceQt::onAcceptError(QAbstractSocket::SocketError /*err*/)
{
}

void NetworkInterfaceQt::onNewConnection()
{
   /* only one connection at a time is allowed. If another connection
    * is active, send a NACK to new client */
   QTcpSocket * newSocket = m_tcpServer->nextPendingConnection();

   if ( newSocket)
   {
      /* accept new client only if no one is already connected */
      if ((m_socket == nullptr) ||
          (! m_socket->isOpen()))
      {
         m_socket = newSocket;
         connect( m_socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()) );
         connect( m_socket, SIGNAL(readyRead()), this, SLOT(onSocketDataReady()) );

         m_commandReply.setSocket( m_socket);

         /* reply with current version */
         QStringList reply = QStringList() << "Welcome to v1.2 beta" <<
                                              "please send password to gain control";
         m_commandReply.sendReplay( Server::COMMAND_ACK, Server::CONNECT,
                                    reply);

         /* notify engine */
         m_serverEngine.onConnectionEstablished();
      }
      else
      {
         /* Notify new client that it can not be served */
         Server::NetworkCommandReply reply;
         reply.setSocket( newSocket);
         reply.sendReplay( Server::CONNECTION_BUSY, Server::CONNECT,
                           "Sorry, someone else has the control.");

         /* close connection */
         newSocket->disconnectFromHost();
         newSocket->abort();
         m_socket = nullptr;
         connect( newSocket, SIGNAL(disconnected()), newSocket, SLOT(deleteLater()) );
      }
   }
}

void NetworkInterfaceQt::onSocketDisconnected()
{
   m_commandReply.setSocket( nullptr);

   /* before destroying socket, disconnect signals from it */
   disconnect( m_socket, SIGNAL(readyRead()), this, SLOT(onSocketDataReady()) );
   m_socket->deleteLater();
   m_socket = nullptr;

   m_serverEngine.onConnectionClosed();
}

void NetworkInterfaceQt::onSocketDataReady()
{
   QByteArray packet = m_socket->readAll();

   T_ASSERT (m_parser != nullptr);
   m_parser->onMessageReceived( packet.constData(), packet.size());
}


void NetworkInterfaceQt::onGrantTimeoutExpired()
{
   m_commandReply.sendReplay( Server::COMMAND_MISSING_PASSWORD, Server::CONNECT,
                              "You did not give your password. Sorry.");
   closeClientConnection();
}


void NetworkInterfaceQt::closeClientConnection()
{
   if (m_socket != nullptr)
   {
      m_socket->disconnectFromHost();
      /* do not call 'abort' to allow goodbye message to arrive */
   }
}


void NetworkInterfaceQt::stopServer()
{
   closeClientConnection();
   m_tcpServer->close();
   m_serverEngine.onConnectionStopped();
}

}  // namespace Server
