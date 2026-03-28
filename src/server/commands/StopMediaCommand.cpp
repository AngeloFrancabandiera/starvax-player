#include "StopMediaCommand.h"

#include <QStringList>
#include "IF_MediaEngineInterface.h"
#include "CommandReply_IF.h"


Server::StopMediaCommand::StopMediaCommand( IF_MediaEngineInterface & mediaEngine_A,
                                            IF_MediaEngineInterface & mediaEngine_B,
                                            CommandReply_IF & replySink) :
   m_mediaEngine_A( mediaEngine_A),
   m_mediaEngine_B( mediaEngine_B),
   m_replySink( replySink)
{
}


bool Server::StopMediaCommand::execute(const QStringList & parameters)
{
   if (parameters.size() >= 1)
   {
      QString line = parameters.at(0);
      bool paramValid = false;

      if ((line == "A") || (line == "A_B"))
      {
         paramValid = true;

         /* one of these will have an effect */
         m_mediaEngine_A.stop();

         /* if no media is currently active nothing happens, but
          * a positive reply is given in any case */
         m_replySink.sendReplay( Server::COMMAND_ACK, STOP_MEDIA, "done");
      }

      if ((line == "B") || (line == "A_B"))
      {
         paramValid = true;

         /* one of these will have an effect */
         m_mediaEngine_B.stop();

         /* see comment for line "A" */
         m_replySink.sendReplay( Server::COMMAND_ACK, STOP_MEDIA, "done");
      }

      if ( ! paramValid)
      {
         m_replySink.sendReplay( Server::COMMAND_INVALID_ARGUMENT, STOP_MEDIA,
                                 "line must be 'A', 'B' or 'A_B'");
      }
   }
   else
   {
      m_replySink.sendReplay( Server::COMMAND_NOT_ENOUGH_ARGS, STOP_MEDIA,
                              "one argument is needed. Can be 'A', 'B' or 'A_B'");
   }

   return true;
}

