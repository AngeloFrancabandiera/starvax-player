#ifndef COMMANDREPLY_IF_H
#define COMMANDREPLY_IF_H

#include <qglobal.h>
class QString;

namespace Server {

const quint8 COMMAND_ACK = 0u;
const quint8 COMMAND_NOT_ENOUGH_ARGS = 1u;
const quint8 COMMAND_MEDIA_NOT_FOUND = 2u;
const quint8 COMMAND_LIGHTSET_NOT_FOUND = 3u;
const quint8 COMMAND_INVALID_ARGUMENT = 4u;
const quint8 COMMAND_UNSUPPORTED = 5u;
const quint8 COMMAND_NOT_GRANTED = 6u;
const quint8 COMMAND_WRONG_PASSWORD = 7u;
const quint8 COMMAND_MISSING_PASSWORD = 8u;

const quint8 CONNECTION_BUSY = 0xf0u;



class CommandReply_IF
{
public:
   virtual ~CommandReply_IF() {}

   /**
    * @brief send a replay code with a message
    * @param replyCode should be one from the ones listed in this file.
    * @param requestCode helps to understand which command the reply refers to
    * @param message may contain an "answer" in text form of explain an error.
    */
   virtual void sendReplay( quint8 replyCode, quint8 requestCode,
                            const QString & message) = 0;

   /**
    * @brief send a replay code with a set of messages
    * @param replyCode should be one from the ones listed in this file.
    * @param requestCode helps to understand which command the reply refers to
    * @param message may contain an "answer" in text form (eg. a list of tracks
    *   or ligh sets), or explain an error.
    */
   virtual void sendReplay( quint8 replyCode, quint8 requestCode,
                            const QStringList & messageList) = 0;
};

}  // namespace Server

#endif // COMMANDREPLY_IF_H
