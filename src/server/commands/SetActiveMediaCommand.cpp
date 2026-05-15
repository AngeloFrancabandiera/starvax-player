#include "SetActiveMediaCommand.h"
#include <QStringList>

#include "MediaAutomation.h"
#include "MediaListModel.h"
#include "CommandReply_IF.h"
#include "testableAssert.h"


Server::SetActiveTrackCommand::
SetActiveTrackCommand( std::array<MediaAutomation *, NUMBER_OF_MEDIA_DECKS> & mediaAutomationSet,
                       std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & mediaListModelSet,
                       CommandReply_IF & replySink) :
   m_mediaAutomationSet( mediaAutomationSet),
   m_mediaListModelSet( mediaListModelSet),
   m_replySink( replySink)
{
}


bool Server::SetActiveTrackCommand::execute(const QStringList & parameters)
{
   QString label;
   QString line;
   m_errorString = QString();
   bool ok = false;

   if (parameters.size() >= 2)
   {
      line = parameters.at(0);
      label = parameters.at(1);

      /* try to activate */
      ok = activateLabelForDeck( label, line);

      if (ok)
      {
         m_replySink.sendReplay( Server::COMMAND_ACK, SET_ACTIVE_MEDIA,
                                 QStringList() << "done");
      }
      else
      {
         m_replySink.sendReplay( Server::COMMAND_MEDIA_NOT_FOUND, SET_ACTIVE_MEDIA,
                                 QStringList() << m_errorString);
      }
   }
   else
   {
      m_replySink.sendReplay( Server::COMMAND_NOT_ENOUGH_ARGS, SET_ACTIVE_MEDIA,
                              QStringList() << "Two arguments required: "
                              << "line ('A' or 'B') and track label");
   }

   return ok;
}

bool Server::SetActiveTrackCommand::activateLabelForDeck( const QString & label,
                                                          const QString & deck)
{
   bool valid = false;
   MediaAutomation * automation = nullptr;
   MediaListModel * model = nullptr;
   QString lineTag;

   int deck_num = Playlist::toDeck(deck.at(0));
   T_ASSERT( deck_num < NUMBER_OF_MEDIA_DECKS);

   automation = m_mediaAutomationSet[deck_num];
   model = dynamic_cast<MediaListModel *>(m_mediaListModelSet[deck_num]);
   lineTag = deck;


   if (model)
   {
      /* line is valid. Search for label in selected line */
      if (model->searchById( label) != QModelIndex())
      {
         automation->activateMediaById( label, false);
         valid = true;
      }
      else
      {
         m_errorString = QString("can't find %1 in line %2").arg(label).arg(lineTag);
      }
   }

   return valid;
}
