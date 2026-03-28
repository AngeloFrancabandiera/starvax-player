#ifndef PLAYMEDIACOMMAND_H
#define PLAYMEDIACOMMAND_H

#include "Command_IF.h"
class IF_MediaEngineInterface;



namespace Server {

class CommandReply_IF;

/**
 * @brief The PlayTrackCommand plays the track that is currently
 *   active. It is not enough to be selected. Nothing happens
 *   if no track has focus.
 */
class PlayMediaCommand : public Command_IF
{
public:
   PlayMediaCommand( IF_MediaEngineInterface & mediaEngine_A,
                     IF_MediaEngineInterface & mediaEngine_B,
                     CommandReply_IF & replySink);

   ~PlayMediaCommand() override{}

   // Command_IF interface
public:
   quint8 operativeCode() const override{
      return PLAY_MEDIA;
   }

   /**
    * @brief plays current track.
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

#endif // PLAYMEDIACOMMAND_H
