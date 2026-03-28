#ifndef PAUSEMEDIACOMMAND_H
#define PAUSEMEDIACOMMAND_H

#include "Command_IF.h"
class IF_MediaEngineInterface;
class iF_MediaEngineInterface;

namespace Server {

class CommandReply_IF;

/**
 * @brief The PauseTrackCommand pauses the track that is currently
 *   active. It is not enough to be selected. Nothing happens
 *   if no track has focus.
 */
class PauseMediaCommand : public Command_IF
{
public:
   PauseMediaCommand( IF_MediaEngineInterface & mediaEngine_A,
                      IF_MediaEngineInterface & mediaEngine_B,
                      CommandReply_IF & replySink);
   ~PauseMediaCommand() override {}

   // Command_IF interface
public:
   quint8 operativeCode() const override{
      return PAUSE_MEDIA;
   }

   /**
    * @brief pauses current track.
    * @param parameters is one string that can be "A", "B" or "A_B"
    * @return
    */
   bool execute(const QStringList & parameters) override;

private:
   IF_MediaEngineInterface & m_mediaEngine_A;
   IF_MediaEngineInterface & m_mediaEngine_B;
   CommandReply_IF & m_replySink;
};

}  // namespace Server

#endif // PAUSEMEDIACOMMAND_H
