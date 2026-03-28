#include "Sequencer_SequenceInstruction.h"

namespace Sequencer {

SequenceInstruction::SequenceInstruction( int srcLineNumber,
                                          SequenceSubCommand command,
                                          double delay) :
   Instruction( srcLineNumber),
   m_command( command),
   m_delayMs( static_cast<int>(delay*1000.))
{
}

int SequenceInstruction::delayMs()
{
   int delay = (m_command == SEQUENCE_STOP) ? DELAY_FOREVER : m_delayMs;

   return delay;
}


}  // namespace Sequencer




