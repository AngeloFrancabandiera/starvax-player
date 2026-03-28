#ifndef SETACTIVETRACKCOMMAND_H
#define SETACTIVETRACKCOMMAND_H

#include "Command_IF.h"
#include <QString>
class MediaAutomation;
class MediaListModel;


namespace Server {

class CommandReply_IF;


/**
 * @brief The SetActiveTrackCommand class sets the active track
 * in the playlist.
 */
class SetActiveTrackCommand : public Command_IF
{
public:
   SetActiveTrackCommand( MediaAutomation & audioVideoAutomation_A,
                          MediaAutomation & audioVideoAutomation_B,
                          MediaListModel & mediaModel_A,
                          MediaListModel & mediaModel_B,
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
   MediaAutomation & m_audioVideoAutomation_A;
   MediaAutomation & m_audioVideoAutomation_B;
   MediaListModel & m_mediaModel_A;
   MediaListModel & m_mediaModel_B;
   CommandReply_IF & m_replySink;

   QString m_errorString;

private:
   bool activateLabelForLine( const QString & label,
                              const QString & line);
};

}  // namespace Server

#endif // SETACTIVETRACKCOMMAND_H
