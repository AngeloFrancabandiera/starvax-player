#include "OpenWebNetEngine.h"
#include "ApplicationSettings.h"
#include "StatusDisplay.h"
#include "testableAssert.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QMap>
#include <QMessageBox>
#include <QRegularExpression>


namespace  {
const quint16 OWN_TCP_PORT = 20000u;
QMap<QString, QString> levelForStringMap;

const QString IP_ADDRESS_PATTERN(
      "((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");

}  // namespace


OpenWebNetEngine::OpenWebNetEngine( const ApplicationSettings & settings,
                                    StatusDisplay * msgDisplay,
                                    QObject *parent) :
   OpenWebNetEngine_IF(parent),
   m_settings(settings),
   m_msgDisplay( msgDisplay),
   m_socket( new QTcpSocket(this))
{
   levelForStringMap.insert( "20%", "2");
   levelForStringMap.insert( "30%", "3");
   levelForStringMap.insert( "40%", "4");
   levelForStringMap.insert( "50%", "5");
   levelForStringMap.insert( "60%", "6");
   levelForStringMap.insert( "70%", "7");
   levelForStringMap.insert( "80%", "8");
   levelForStringMap.insert( "90%", "9");
   levelForStringMap.insert( "100%", "10");

   connect( m_socket, & QTcpSocket::errorOccurred,
            this, & OpenWebNetEngine::onTcpError );
}

OpenWebNetEngine::~OpenWebNetEngine()
{
}

void OpenWebNetEngine::turnOn(const QString & where)
{
   checkNotEmpty( where);
   sendOwnMessage(QString("*1*1*%1##").arg(where));
}

void OpenWebNetEngine::turnOff( const QString & where)
{
   checkNotEmpty( where);
   sendOwnMessage(QString("*1*0*%1##").arg(where));
}

void OpenWebNetEngine::dimmer(int level, const QString & where)
{
   T_ASSERT( (level >= 1) && (level <= 10));

   checkNotEmpty( where);

   if (level == 1)
   {
      // OFF
      turnOff( where);
   }
   else
   {
      sendOwnMessage(QString("*1*%1*%2##").arg(level).arg(where));
   }
}

void OpenWebNetEngine::sendOwnMessage(const QString & message)
{
   ApplicationSettings::OpenWebNetSettings ownSetting =
         m_settings.getOpenWebNetSettings();

   if (isIpValid(ownSetting.serverIpAddress))
   {
      m_socket->abort();
      m_socket->connectToHost( QHostAddress(ownSetting.serverIpAddress), OWN_TCP_PORT);

      /* write command */
      qint64 writeSize = m_socket->write( message.toLatin1());

      QString log = QString("Open-Web-Net: sending %1").arg(message);

      if (writeSize < message.length())
      {
         log.append(tr(" - FAIL "));
      }

      m_msgDisplay->showMessage( log, StatusDisplay::INFORMATION);
   }
   else
   {
      QMessageBox::warning( nullptr, "Open Web Net",
                            tr("Invalid IP address. You may want to check"
                               " Open Web Net settings in \"Show>>Options\" menu."));
   }
}


QString OpenWebNetEngine::levelForString(const QString & guiLevel)
{
   return levelForStringMap.value( guiLevel, "60%");
}

bool OpenWebNetEngine::isIpValid(const QString & ipString)
{
   QRegularExpression ipRegExp( IP_ADDRESS_PATTERN);
   QRegularExpressionMatch match = ipRegExp.match(ipString);

   return match.hasMatch() && (ipString != "0.0.0.0");
}

void OpenWebNetEngine::onTcpError( const QAbstractSocket::SocketError & error)
{
   m_msgDisplay->showMessage( tr("Open Web Net TCP Error: %1").arg((int)error),
                              StatusDisplay::WARNING);
}

void OpenWebNetEngine::checkNotEmpty( const QString & where)
{
   if (where == QString())
   {
      m_msgDisplay->showMessage( tr("Invalid zone."), StatusDisplay::WARNING);
   }
}
