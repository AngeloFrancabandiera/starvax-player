#ifndef OPENWEBNETENGINE_H
#define OPENWEBNETENGINE_H

#include <QObject>
#include <QAbstractSocket>
#include <OpenWebNetEngine_IF.h>

class ApplicationSettings;
class QTcpSocket;
class StatusDisplay;


class OpenWebNetEngine : public OpenWebNetEngine_IF
{
   Q_OBJECT
public:
   explicit OpenWebNetEngine( const ApplicationSettings & settings,
                              StatusDisplay * msgDisplay,
                              QObject *parent = nullptr);

   ~OpenWebNetEngine();

public slots:
   void turnOn(const QString & where);
   void turnOff(const QString & where);

   /**
    * @brief send an OWN dimmer command
    * @param level: 1 -> off; 2-10: 20% to 100%
    * @param where is not checked for syntax
    */
   void dimmer( int level, const QString & where);

private:
   const ApplicationSettings & m_settings;
   StatusDisplay * m_msgDisplay;
   QTcpSocket * m_socket;

private:
   void sendOwnMessage(const QString & message);
   QString levelForString( const QString & guiLevel);
   bool isIpValid( const QString & ipString);
   void checkNotEmpty( const QString & where);

private slots:
   void onTcpError(const QAbstractSocket::SocketError & error);
};

#endif // OPENWEBNETENGINE_H
