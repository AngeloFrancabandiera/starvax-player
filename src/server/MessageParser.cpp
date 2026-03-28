#include "MessageParser.h"
#include <QStringList>

#include "testableAssert.h"
#include "MessageEngine.h"

namespace  {
const quint8 TERMINATOR = static_cast<quint8>(0xFF);

/** two TERMINATORs indicate end of packet */
const int TERMINATOR_SEQUENCE_SIZE = 2;
}  // namespace



Server::MessageParser::MessageParser() :
   m_terminatorCounter(0)
{
}

Server::MessageParser::~MessageParser()
{
}


void Server::MessageParser::addEngine(Server::MessageEngine * engine)
{
   T_ASSERT (engine != nullptr);
   m_engineList << engine;
}


void Server::MessageParser::onMessageReceived( const char * message, unsigned size)
{
   /* accumulate chars in partial string, until sequence 0xFF-0xFF is found */
   for (unsigned i=0u; i < size; i++)
   {
      m_partialMessage << message[i];

      if (message[i] == static_cast<char>(TERMINATOR))
      {
         ++m_terminatorCounter;
      }
      else
      {
         m_terminatorCounter = 0;
      }

      if (m_terminatorCounter == TERMINATOR_SEQUENCE_SIZE)
      {
         parseCompleteReqMessage();

         m_partialMessage.clear();
         m_terminatorCounter = 0;
      }
   }
}

/**
 * @brief parse internal partial message assuming that it contains one
 * complete request message
 */
void Server::MessageParser::parseCompleteReqMessage()
{
   T_ASSERT( m_partialMessage.size() >= 2);

   quint8 opCode = static_cast<quint8>(m_partialMessage[0]);
   quint8 numberOfParams = static_cast<quint8>(m_partialMessage[1]);

   /* cursor of 'messeage' bytes. Parameters start at position 2 */
   int messageIndex = 2;
   int availableChars = m_partialMessage.size();

   QStringList paramList;
   QString param;

   for (quint8 i=0; (i < numberOfParams) && (messageIndex < availableChars); i++)
   {
      param.clear();

      /* collect data up to terminator */
      while( (messageIndex < availableChars) &&
             (m_partialMessage.at(messageIndex) != '\0'))
      {
         param.append( m_partialMessage.at(messageIndex));
         ++messageIndex;
      }

      /* skip after terminator, if one has been added */
      if (messageIndex < availableChars)   /* implies that a '\0' was found */
      {
         ++messageIndex;
      }

      paramList << param;
   }

   notifyAllEngines( opCode, paramList);
}


void Server::MessageParser::notifyAllEngines( quint8 opCode,
                                              const QStringList & params)
{
   foreach (Server::MessageEngine * engine, m_engineList)
   {
      engine->onMessageReceived( opCode, params);
   }
}


