#include "Sequencer_ScriptParser.h"
#include "Sequencer_LineParser.h"
#include "Sequencer_InstructionFactory_IF.h"

namespace Sequencer {

ScriptParser::ScriptParser( LineParser & lineParser,
                            InstructionFactory_IF & instructionFactory) :
   m_lineParser( lineParser),
   m_instructionFactory( instructionFactory)
{
}

void ScriptParser::parse( const QStringList & script)
{
   bool isInstruction;

   m_errorList.clear();
   m_instructionSequence.clear();

   for( int lineNumb = 1; lineNumb <= script.size(); lineNumb++)
   {
      isInstruction = m_lineParser.parseLine( script.at(lineNumb - 1));

      /* chekc if an error must be reported */
      if ( m_lineParser.getLineKind() == Sequencer::LINE_INVALID)
      {
         Error error( lineNumb, m_lineParser.errorString());
         m_errorList << error;
      }

      if (isInstruction)
      {
         m_instructionSequence << m_instructionFactory.buildInstructionForLine( m_lineParser, lineNumb);
      }
   }
}



}  // namespace Sequencer
