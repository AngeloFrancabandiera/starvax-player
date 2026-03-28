#include "GetMediaListCommand.h"
#include "MediaListModel.h"
#include "CommandReply_IF.h"
#include "modelViewRules.h"


Server::GetMediaListCommand::GetMediaListCommand( MediaListModel & mediaModel_A,
                                                  MediaListModel & mediaModel_B,
                                                  CommandReply_IF & replySink) :
   m_mediaModel_A( mediaModel_A),
   m_mediaModel_B( mediaModel_B),
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

      if (param == "A")
      {
         tag = 'A';
      }
      else if (param == "B")
      {
         tag = 'B';
      }
   }

   return tag;
}

MediaListModel * Server::GetMediaListCommand::selectModel( char modelTag)
{
   MediaListModel * model = nullptr;

   if (modelTag == 'A')
   {
      model = & m_mediaModel_A;
   }
   else if (modelTag == 'B')
   {
      model = & m_mediaModel_B;
   }

   return model;
}

