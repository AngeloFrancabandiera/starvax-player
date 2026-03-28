#include "Sequencer_Instruction.h"




Sequencer::Instruction::Instruction(int srcLineNumber, LineKind kind):
   m_srcLineNumber( srcLineNumber),
   m_nextInstruction( nullptr),
   m_kind(kind)
{}

Sequencer::Instruction *Sequencer::Instruction::next() {
   return m_nextInstruction;
}

void Sequencer::Instruction::setNext(Sequencer::Instruction * next)
{
   m_nextInstruction = next;
}

int Sequencer::Instruction::delayMs() {
   return 0;
}

int Sequencer::Instruction::sourceLineNumber() const {
   return m_srcLineNumber;
}
