#ifndef SEQUENCER_ENTRYINSTRUCTION_H
#define SEQUENCER_ENTRYINSTRUCTION_H

#include "Sequencer_Instruction.h"


namespace Sequencer {

class EntryInstruction : public Instruction
{
public:
   EntryInstruction( const QString & label, int srcLineNumber);

   const QString & label() const {
      return m_label;
   }

private:
   QString m_label;
};

}  //namespace Sequencer

#endif // SEQUENCER_ENTRYINSTRUCTION_H
