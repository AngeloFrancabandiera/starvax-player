#include "Sequencer_EntryInstruction.h"

namespace Sequencer {


EntryInstruction::EntryInstruction( const QString & label, int srcLineNumber)  :
   Instruction( srcLineNumber, LINE_ENTRY),
   m_label( label)
{
}

}  // namespace Sequencer
