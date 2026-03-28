#ifndef MESSAGEENGINE_H
#define MESSAGEENGINE_H

#include <QObject>

namespace Server {


/**
 * @brief The MessageEngine class is the virtual interface
 *   to react to messages directed to server engine.
 * This is invoked by \p MessageParser
 */
class MessageEngine : public QObject
{
   Q_OBJECT
public:
   MessageEngine( QObject * parent = nullptr) :
      QObject( parent)
   {}
   virtual ~MessageEngine() {}

   /**
    * @brief to be called when a message from the network arrives
    * @param opCode identifies a command
    * @param params is a set of string extracted from message by null termination.
    * @return true if the message is executed correctly.
    */
   virtual bool onMessageReceived(quint8 opCode, const QStringList & params) = 0;
};

}  // namespace Server

#endif // MESSAGEENGINE_H
