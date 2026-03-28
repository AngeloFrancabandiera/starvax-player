#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <QList>
#include <QString>

namespace Server {

class MessageEngine;

/**
 * @brief The MessageParser class parses messages from the network. When
 * a valid command is decoded, registred engines are notified.
 *
 * As messages come from a TCP socket, we cannot assume that each packet
 * is a full message with a command from client.
 */
class MessageParser
{
public:
   MessageParser();
   ~MessageParser();

   /**
    * @brief add a new \a ServerEngine that will be notified when a
    *   command is decoded
    * @param engine must not be nullptr
    */
   void addEngine( MessageEngine * engine);

   /**
    * @brief to be called whenever a message from Network arrives.
    * @param message holds data payload.
    * @param size is the size of \p message.
    *
    * The message may hold exactly one command request, or may contain part
    * of a request (that TCP layer has split into packets), or even more
    * than one command in one packet.
    * Packets are accumulated internally untill the sequence 0xFF-0xFF is found.
    * When this happens, data accumulated so far are processed to extract one
    * command request. The form of request is:
    * - OP-CODE (1 byte range 1-250)
    * - NUMBER_OF_ARGUMENTS (1 byte range 0-250)
    * - a list of null terminated ascii strings
    * - final tag 0xFF-0xFF
    */
   void onMessageReceived( const char * message, unsigned size);

private:
   struct PartialMessage;

private:
   QList<MessageEngine *> m_engineList;
   QList<char> m_partialMessage;
   int m_terminatorCounter;  // counter of consequtive 0xFF characters

private:
   void parseCompleteReqMessage();
   void notifyAllEngines( quint8 opCode,
                          const QStringList & params);
};

}  // namespace Server

#endif // MESSAGEPARSER_H
