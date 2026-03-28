#ifndef SEQUENCER_ENDINSTRUCTION_H
#define SEQUENCER_ENDINSTRUCTION_H

#include "Sequencer_Instruction.h"


namespace Sequencer {

/** This instruction closes a 'repeat' instruction.
 * His next instruction is always a 'repeat' instruction. Such
 * 'repeat' instruction willl decide wheter to enter the loop
 * or go to the next instruction.
 */
class EndInstruction : public Instruction
{
public:
   EndInstruction( int srcLineNumber);

};

}  // namespace Sequencer
#endif // SEQUENCER_ENDINSTRUCTION_H
