#ifndef SETACTIVETRACKCOMMAND_H
#define SETACTIVETRACKCOMMAND_H

#include "Command_IF.h"
#include <QString>
#include <array>

class MediaAutomationSet;
class MediaListModel;
class QAbstractListModel;


namespace Server {

class CommandReply_IF;


/**
 * @brief The SetActiveTrackCommand class sets the active track
 * in the playlist.
 */
class SetActiveTrackCommand : public Command_IF
{
public:
   SetActiveTrackCommand( MediaAutomationSet & mediaAutomationSet,
                          std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & mediaListModelSet,
                          CommandReply_IF & replySink);
   ~SetActiveTrackCommand() override {}

   // Command_IF interface
public:
   quint8 operativeCode() const override {
      return SET_ACTIVE_MEDIA;
   }

   /**
    * @brief this commands sets the current track for line A or line B
    * @param parameters has two strings.
    *   The first is "A" or "B", the second is track label
    * @return True if given track exists for given line
    */
   bool execute(const QStringList & parameters) override;

private:
   MediaAutomationSet & m_mediaAutomationSet;
   std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & m_mediaListModelSet;
   CommandReply_IF & m_replySink;

   QString m_errorString;

private:
   bool activateLabelForDeck( const QString & label,
                              const QString & deck);
};

}  // namespace Server

#endif // SETACTIVETRACKCOMMAND_H
