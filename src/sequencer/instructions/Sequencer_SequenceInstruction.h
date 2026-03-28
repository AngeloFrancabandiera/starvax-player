#ifndef SEQUENCER_SEQUENCEINSTRUCTION_H
#define SEQUENCER_SEQUENCEINSTRUCTION_H

#include "Sequencer_Instruction.h"


namespace Sequencer {

/** This command is used to control the flow of the sequence
 * by blocking it or by pausing for a fixed time.
 * Method 'execute' does nothing, but the delay after execution
 * is defined by overriding 'delayMs'
 */
class SequenceInstruction : public Instruction
{
public:
   SequenceInstruction( int srcLineNumber,
                        SequenceSubCommand command,
                        double delay);

   // Instruction interface
public:
   int delayMs() override;

private:
   SequenceSubCommand m_command;
   int m_delayMs;
};

}  // namespace Sequencer

#endif // SEQUENCER_SEQUENCEINSTRUCTION_H
