#ifndef SEQUENCER_PLAYLISTINSTRUCTION_H
#define SEQUENCER_PLAYLISTINSTRUCTION_H

#include "Sequencer_Instruction.h"

class MediaAutomation;

namespace Sequencer {

/** child class of \a InstructionEvent for playlist instruction */
class PlaylistInstructionEvent : public InstructionEvent
{
   Q_OBJECT
public:
   PlaylistInstructionEvent( MediaAutomation & automation,
                             QObject * parent = nullptr);

private slots:
   void onTrackFinished();
};


/**
 * This instruction controls the playback of a media,
 * (audio/video) or image. The constructor accepts the engine
 * relative to the line (A or B) that is controlled.
 */
class PlaylistInstruction : public Instruction
{
public:
   /**
    * @param srcLineNumber: line number of instruction in source file
    * @param mediaEngine: engine for the line (A or B) to be controlled
    * @param label: media to be activated
    */
   PlaylistInstruction( int srcLineNumber,
                        MediaAutomation & automation,
                        Sequencer::PlaylistSubCommand command,
                        const QString & label = QString(),
                        int volume = -1);

   ~PlaylistInstruction() override {}

   // Instruction interface
public:
   void execute(const Instruction * previous = nullptr) override;
   int delayMs() override;
   InstructionEvent* eventNotifier() override {
      return & m_events;
   }

private:
   MediaAutomation & m_automation;
   Sequencer::PlaylistSubCommand m_command;
   QString m_label;
   int m_volume;
   PlaylistInstructionEvent m_events;
};

}  // namespace Sequencer

#endif // SEQUENCER_PLAYLISTINSTRUCTION_H
