#ifndef SEQUENCER_INSTRUCTIONFACTORY_IF_H
#define SEQUENCER_INSTRUCTIONFACTORY_IF_H

#include "Sequencer.h"
#include "Sequencer_LineParser.h"

namespace Sequencer {

class Instruction;


class InstructionFactory_IF
{
public:
   virtual ~InstructionFactory_IF() {}

   /**
    * build an instruction for a line that has been parsed in a script.
    * @param parser is a parser that has just parsed a line. It is used
    *    to retrieve info about the last line that has been parsed.
    * @return the instruction. May return NULL if last parse operation
    *    found an error
    */
   virtual Instruction * buildInstructionForLine( const LineParser & parser,
                                                  int sourceLineNumber) = 0;

};

}  // namespace Sequencer

#endif // SEQUENCER_INSTRUCTIONFACTORY_IF_H
