#include "PauseMediaCommand.h"

#include <QStringList>
#include "IF_MediaEngineInterface.h"
#include "IF_MediaEngineInterface.h"
#include "CommandReply_IF.h"


Server::PauseMediaCommand::PauseMediaCommand( IF_MediaEngineInterface & mediaEngine_A,
                                              IF_MediaEngineInterface & mediaEngine_B,
                                              CommandReply_IF & replySink) :
   m_mediaEngine_A( mediaEngine_A),
   m_mediaEngine_B( mediaEngine_B),
   m_replySink( replySink)
{
}


bool Server::PauseMediaCommand::execute(const QStringList & parameters)
{
   if (parameters.size() >= 1)
   {
      QString line = parameters.at(0);
      bool paramValid = false;

      if ((line == "A") || (line == "A_B"))
      {
         paramValid = true;

         m_mediaEngine_A.pause();

         /* if no track is currently active nothing happens, but
          * a positive reply is given in any case */
         m_replySink.sendReplay( Server::COMMAND_ACK, PAUSE_MEDIA,
                                 "done");
      }

      if ((line == "B") || (line == "A_B"))
      {
         paramValid = true;

         m_mediaEngine_B.pause();

         /* see comment for line "A" */
         m_replySink.sendReplay( Server::COMMAND_ACK, PAUSE_MEDIA,
                                 "done");
      }

      if ( ! paramValid)
      {
         m_replySink.sendReplay( Server::COMMAND_INVALID_ARGUMENT, PAUSE_MEDIA,
                                 "line must be 'A', 'B' or 'A_B'");
      }
   }
   else
   {
      m_replySink.sendReplay( Server::COMMAND_NOT_ENOUGH_ARGS, PAUSE_MEDIA,
                              "one argument is needed. Can be 'A', 'B' or 'A_B'");
   }

   return true;
}

