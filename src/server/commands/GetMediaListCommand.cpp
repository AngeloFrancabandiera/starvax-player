#include "GetMediaListCommand.h"
#include "MediaListModel.h"
#include "CommandReply_IF.h"
#include "modelViewRules.h"
#include "PlaylistDecks.h"


Server::GetMediaListCommand::GetMediaListCommand( std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & mediaListModelSet,
                                                  CommandReply_IF & replySink) :
   m_mediaListModelSet( mediaListModelSet),
   m_replySink( replySink)
{
}


bool Server::GetMediaListCommand::execute(const QStringList & parameters)
{
   bool result = false;

   /* select one model according to parameter */
   MediaListModel * model = nullptr;

   char modelTag = selectModelTag( parameters);
   model = selectModel( modelTag);

   if (model)
   {
      result = true;

      QStringList trackList;
      // put model tag before the track list
      trackList << QString("%1").arg(modelTag);

      int numberOfTracks = model->rowCount();

      for (int i=0; i < numberOfTracks; i++)
      {
         trackList << model->index(i).data( modelViewRules::StringId).toString();
      }

      m_replySink.sendReplay( Server::COMMAND_ACK, GET_MEDIA_LIST,
                              trackList);
   }
   else
   {
      m_replySink.sendReplay( Server::COMMAND_INVALID_ARGUMENT, GET_MEDIA_LIST,
                              "Media line must be 'A' or 'B'");
   }


   return result;
}

char Server::GetMediaListCommand::selectModelTag(const QStringList & parameters)
{
   char tag = ' ';

   if (parameters.size() >= 1)
   {
      QString param = parameters.at(0);

      if ((param.at(0) >= 'A') && (param.at(0) < (QChar('A' + NUMBER_OF_MEDIA_DECKS))))
      {
         tag = param.at(0).unicode();
      }
   }

   return tag;
}

MediaListModel * Server::GetMediaListCommand::selectModel( char modelTag)
{
   MediaListModel * model = nullptr;
   int deck = Playlist::toDeck(modelTag);

   if (deck <= NUMBER_OF_MEDIA_DECKS)
   {
      model = dynamic_cast<MediaListModel *>(m_mediaListModelSet[deck]);
   }

   return model;
}

