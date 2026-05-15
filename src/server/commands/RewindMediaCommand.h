#ifndef REWINDMEDIACOMMAND_H
#define REWINDMEDIACOMMAND_H

#include "Command_IF.h"
class IF_MediaEngineInterface;
class iF_MediaEngineInterface;

namespace Server {

class CommandReply_IF;

/**
 * @brief The RewindMediaCommand rewinds the media that is currently
 *   active. It is not enough to be selected. Nothing happens
 *   if no media has focus.
 */
class RewindMediaCommand : public Command_IF
{
public:
   RewindMediaCommand( std::array<IF_MediaEngineInterface *, NUMBER_OF_MEDIA_DECKS> & mediaEngineSet,
                       CommandReply_IF & replySink);

   ~RewindMediaCommand() override {}

   // Command_IF interface
public:
   quint8 operativeCode() const override{
      return REWIND_MEDIA;
   }

   /**
    * @brief rewinds current media.
    * @param parameters is one string that can be "A", "B" or "A_B"
    * @return
    */
   bool execute(const QStringList & parameters) override;

private:
   std::array<IF_MediaEngineInterface *, NUMBER_OF_MEDIA_DECKS> & m_mediaEngineSet;
   CommandReply_IF & m_replySink;
};

}  // namespace Server

#endif // REWINDMEDIACOMMAND_H
