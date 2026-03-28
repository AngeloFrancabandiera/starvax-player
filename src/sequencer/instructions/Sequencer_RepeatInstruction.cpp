#include "Sequencer_RepeatInstruction.h"
#include "SequenceEditorGui.h"

namespace Sequencer {

RepeatInstruction::RepeatInstruction( int srcLineNumber, int repeatNumber,
                                      SequenceEditorGui & gui) :
   Instruction( srcLineNumber, LINE_REPEAT),
   m_totalRepeatNumber( repeatNumber),
   m_currentRepeatNumber(0),
   m_gui( gui),
   m_postLoopInstruction( nullptr)
{
}


void RepeatInstruction::setInstructionOutsideLoop(Instruction *instruction)
{
   m_postLoopInstruction = instruction;
}


void RepeatInstruction::execute( const Instruction *previous)
{
   /* check if loop is being entered for the first time */
   if ((previous == nullptr) ||
       (previous->instructionKind() != LINE_END))
   {
      /* entering loop: reset counter */
      m_currentRepeatNumber = 0;
   }
   else
   {
      m_currentRepeatNumber++;
   }

   m_gui.displayStatusMessage( QObject::tr("repeat line <b>%1</b>, count: <b>%2</b>").
                               arg(sourceLineNumber()).arg( m_currentRepeatNumber+1));
}


Instruction *RepeatInstruction::next()
{
   Instruction * nextInstr = Instruction::next();

   if ((m_totalRepeatNumber != REPEAT_FOREVER) &&
       (m_currentRepeatNumber >= m_totalRepeatNumber))
   {
      /* end of iterations */
      nextInstr = m_postLoopInstruction;
      m_gui.displayStatusMessage("");
   }

   return  nextInstr;
}


}  // namespace Sequencer

