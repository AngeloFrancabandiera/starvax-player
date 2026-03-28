#ifndef NETWORKINTERFACE
#define NETWORKINTERFACE

#include <QObject>

namespace Server {

class MessageParser;

/**
 * @brief The NetworkInterface class wraps the network connection
 * used to listen for remote commands.
 */
class NetworkInterface : public QObject
{
   Q_OBJECT
public:
   NetworkInterface( QObject * parent = nullptr) :
      QObject( parent)
   {}

   /**
    * @brief setParser
    * @param parser
    */
   virtual void setParser( MessageParser * parser) = 0;

public slots:
   /**
    * @brief start listening for clients. Only one client per time is allowed.
    */
   virtual void startServer() = 0;

   /**
    * @brief shut down server functionality. Should be called when application exits.
    */
   virtual void stopServer() = 0;

   /**
    * @brief close the connection with the current client.
    */
   virtual void closeClientConnection() = 0;
};

}  // namespace Server


#endif // NETWORKINTERFACE

