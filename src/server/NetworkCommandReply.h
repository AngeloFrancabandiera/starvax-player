#ifndef NETWORKCOMMANDREPLY_H
#define NETWORKCOMMANDREPLY_H

#include "CommandReply_IF.h"
class QIODevice;

namespace Server {


class NetworkCommandReply : public Server::CommandReply_IF
{
public:
   NetworkCommandReply();
   ~NetworkCommandReply() override {}

   // CommandReply_IF interface
public:
   /**
    * @brief this implementation prints the reply code in big endian format,
    *   follwed by the \p message in plain ascii from byte 2, terminated by '\0'
    * @param replyCode
    * @param requestCode helps to understand which command the reply refers to
    * @param message
    */
   void sendReplay( quint8 replyCode, quint8 requestCode,
                    const QString & message) override;

   /**
    * @brief this implementation prints the reply code in big endian format;
    *   from byte 2 the \p messageList is printed as a sequence of nullptr terminated
    *   strings. At the end of the list a double nullptr charcter is written.
    * @param replyCode
    * @param requestCode helps to understand which command the reply refers to
    * @param message
    */
   void sendReplay( quint8 replyCode, quint8 requestCode,
                    const QStringList & messageList) override;

public:
   /**
    * @brief set the socket where to write data. When socket is closed or
    *   deleted, this function must be called with nullptr as parameter.
    * @param socket
    */
   void setSocket( QIODevice * socket) {
      m_device = socket;
   }

private:
   QIODevice * m_device;
};

}  // namespace Server

#endif // NETWORKCOMMANDREPLY_H
