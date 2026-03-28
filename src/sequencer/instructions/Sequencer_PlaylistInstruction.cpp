#include "Sequencer_PlaylistInstruction.h"
#include "MediaAutomation.h"

namespace Sequencer {

PlaylistInstruction::PlaylistInstruction(int srcLineNumber,
                                          MediaAutomation & automation,
                                          PlaylistSubCommand command,
                                          const QString &label, int volume) :
   Instruction ( srcLineNumber, LINE_PLAYLIST),
   m_automation( automation),
   m_command( command),
   m_label( label),
   m_volume( volume),
   m_events( automation, nullptr)
{
}


void PlaylistInstruction::execute( const Instruction * /*previousInstruction*/)
{
   switch (m_command)
   {
   default:
   case PLAYLIST_NONE:
      break;

   case PLAYLIST_PLAY:
   case PLAYLIST_PLAY_ALL:
      /* activate and play immediately */
      m_automation.activateMediaById( m_label, true, true);
      break;
   case PLAYLIST_SHOW:
      /* activate and play immediately */
      m_automation.activateMediaById( m_label, true, true);
      break;

   case PLAYLIST_PAUSE:
      m_automation.pause();
      break;

   case PLAYLIST_STOP:
   case PLAYLIST_HIDE:
      m_automation.stop();
      break;

   case PLAYLIST_SET_TOP:
      m_automation.setOnTop( true);
      break;

   case PLAYLIST_SET_BOTTOM:
      m_automation.setOnTop( false);
      break;

   case PLAYLIST_REWIND:
      m_automation.rewind();
      break;

   case PLAYLIST_FADEOUT:
      m_automation.fadeout();
      break;

   case PLAYLIST_VOLUME:
      if (m_volume == -1) {
         m_automation.applyDefaultVolume();
      }
      else {
         m_automation.setVolume( m_volume);
      }
      break;

   case PLAYLIST_WAIT:
       // ??? do nothing. This object will send signal at the right time
       break;
   }
}

int PlaylistInstruction::delayMs()
{
   int delay = 0; /* default case */

   if ((m_command == PLAYLIST_WAIT) || (m_command == PLAYLIST_PLAY_ALL))
   {
      delay = DELAY_UNTIL_EVENT;
   }

   return delay;
}


PlaylistInstructionEvent::PlaylistInstructionEvent(MediaAutomation& automation, QObject* parent) :
   InstructionEvent(parent)
{
   connect( & automation, & MediaAutomation::trackFinished,
            this, &PlaylistInstructionEvent::onTrackFinished);
}


void PlaylistInstructionEvent::onTrackFinished()
{
   emit notifyEvent( Media_trackFinished);
}

} // namespace Sequencer

