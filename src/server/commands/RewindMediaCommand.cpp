#include "RewindMediaCommand.h"

#include <QStringList>
#include "IF_MediaEngineInterface.h"
#include "IF_MediaEngineInterface.h"
#include "CommandReply_IF.h"
#include "PlaylistDecks.h"


Server::RewindMediaCommand::RewindMediaCommand( std::array<IF_MediaEngineInterface *, NUMBER_OF_MEDIA_DECKS> & mediaEngineSet,
                                                CommandReply_IF & replySink) :
   m_mediaEngineSet( mediaEngineSet),
   m_replySink( replySink)
{
}


bool Server::RewindMediaCommand::execute(const QStringList & parameters)
{
   if (parameters.size() >= 1)
   {
      QString deck = parameters.at(0);
      int deck_num = Playlist::toDeck( deck.at(0));
      bool paramValid = false;

      if (deck_num < NUMBER_OF_MEDIA_DECKS)
      {
         paramValid = true;

         m_mediaEngineSet[deck_num]->rewind();

         /* if no media is currently active nothing happens, but
          * a positive reply is given in any case */
         m_replySink.sendReplay( Server::COMMAND_ACK, REWIND_MEDIA, "done");
      }

      if ( ! paramValid)
      {
         m_replySink.sendReplay( Server::COMMAND_INVALID_ARGUMENT, REWIND_MEDIA,
                                 "line must be 'A', 'B', ...");
      }
   }
   else
   {
      m_replySink.sendReplay( Server::COMMAND_NOT_ENOUGH_ARGS, REWIND_MEDIA,
                              "one argument is needed. Can be 'A', 'B', ...");
   }

   return true;
}

