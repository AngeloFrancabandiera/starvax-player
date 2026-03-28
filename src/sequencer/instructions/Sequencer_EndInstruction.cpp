#include "Sequencer_EndInstruction.h"

namespace Sequencer {


EndInstruction::EndInstruction( int srcLineNumber) :
   Instruction (srcLineNumber, LINE_END)
{
}


}  // namespace Sequencer

