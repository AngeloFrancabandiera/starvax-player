#ifndef SEQUENCER_REPEATINSTRUCTION_H
#define SEQUENCER_REPEATINSTRUCTION_H

#include "Sequencer_Instruction.h"

class SequenceEditorGui;

namespace Sequencer {

/**
 * This instruction is used to execute a loop a given number of times.
 * Method \a RepeatInstruction::execute uses 'previous' parameter to
 * detect if we're entering the loop for the first time or if this is
 * an iteration.
 * The instruction to be executed when the loop is over must be defined
 * by \a RepeatInstruction::setInstructionOutsideLoop
 */
class RepeatInstruction : public Instruction
{
public:
   RepeatInstruction( int srcLineNumber, int repeatNumber,
                      SequenceEditorGui & gui);

   /** set the instruction to jump to when the number of iterations are over */
   void setInstructionOutsideLoop( Instruction * instruction);

   /**
    * This is a facility mainly used by GUI to show the progress of cycles
    * @return the current number of iterations
    */
   int currentCount() const {
      return m_currentRepeatNumber;
   }

   // Instruction interface
public:
   void execute(const Instruction * previous) override;

   /** @return the next instruction (stored in base calss) or the
    *     instruction set by \a setInstructionOutsideLoop, according to loop count
    */
   Instruction *next() override;

private:
   int m_totalRepeatNumber;
   int m_currentRepeatNumber;
   SequenceEditorGui & m_gui;
   Instruction * m_postLoopInstruction;
};

}  // namespace Sequencer

#endif // SEQUENCER_REPEATINSTRUCTION_H
