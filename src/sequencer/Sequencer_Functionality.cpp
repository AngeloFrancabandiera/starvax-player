#include "Sequencer_Functionality.h"
#include "testableAssert.h"

#include "SequenceEditorGui.h"
#include "Sequencer_ScriptSequencer.h"
#include "Sequencer_Runner.h"
#include "Sequencer_ScriptParser.h"

#include "Sequencer_Instruction.h"
#include "Sequencer_EntryInstruction.h"


namespace Sequencer {

Functionality::Functionality( SequenceEditorGui & gui,
                              ScriptSequencer & sequencer,
                              Runner & runner,
                              ScriptParser & parser,
                              QObject * parent) :
   QObject( parent),
   m_gui( gui),
   m_sequencer( sequencer),
   m_runner( runner),
   m_parser( parser)
{
   connect( & m_gui, & SequenceEditorGui::runSequenceRequest,
            this, & Functionality::resume);
   connect( & m_gui, & SequenceEditorGui::pauseSequenceRequest,
            this, & Functionality::stop);
   connect( & m_gui, & SequenceEditorGui::parseSequenceRequest,
            this, & Functionality::parse);
   connect( & m_gui, & SequenceEditorGui::activateLineRequest,
            this, & Functionality::setActiveInstructionByLine);

   m_entryLabelsModel.setStringList( m_entryLabels);
}

void Functionality::resume()
{
   m_runner.resume();
}

void Functionality::stop()
{
   m_runner.stop();
}

void Functionality::setActiveInstructionByLine(int line)
{
   int validLine = m_runner.setCurrentInstructionByLine( line);
   m_gui.setActiveLine( validLine);
   /* status from previous command is not valid anymore */
   m_gui.displayStatusMessage("");
}

void Functionality::activateEntry(const QString &label, bool resume)
{
   int lineNumber = lineForEntryWithLabel(label);

   if (lineNumber != -1)
   {
      setActiveInstructionByLine( lineNumber);

      if (resume)
      {
         m_runner.resume();
      }
   }
   else
   {
      m_gui.displayStatusMessage(tr("Can't find label <i>%1</i> in sequencer.").arg(label));
   }
}



void Functionality::parse()
{
   m_gui.clearErrorList();

   /* parse source */
   m_parser.parse( m_gui.scriptContent().split('\n'));

   /* syntax errors */
   reportLineErrors();

   /* proceed in case no errors are there */
   if (m_parser.errorList().isEmpty())
   {
      bool ok;

      ok = m_sequencer.checkEntries( m_parser.instructionSequence());
      if (ok == false)
      {
         reportScriptErrors();
      }

      ok = m_sequencer.makeSequenceInstructionList( m_parser.instructionSequence());

      if (ok)
      {
         m_runner.onScriptRebuilt();
         updateEntryList();

         /* script is valid and aligned to compiled instructions */
         m_gui.setCodeParsed( true);
      }
      else
      {
         reportScriptErrors();
      }
   }
}


void Functionality::reportLineErrors()
{
   foreach( ScriptParser::Error err, m_parser.errorList())
   {
      m_gui.addError( err.line, err.error);
   }
}

void Functionality::reportScriptErrors()
{
   foreach( Sequencer::ScriptSequencer::Issue * issue, m_sequencer.issues())
   {
      T_ASSERT( issue != nullptr);
      T_ASSERT( issue->refInstruction != nullptr);

      m_gui.addError( issue->refInstruction->sourceLineNumber(),
                      issue->issueDescription);
   }
}

void Functionality::updateEntryList()
{
   m_entryLabels.clear();
   m_entryLines.clear();

   for( Instruction * i : m_parser.instructionSequence())
   {
      if (i->instructionKind() == LINE_ENTRY)
      {
         EntryInstruction * entryInst = dynamic_cast<EntryInstruction *>(i);
         T_ASSERT( entryInst != nullptr);

         m_entryLabels << entryInst->label();
         m_entryLines<< entryInst->sourceLineNumber();
      }
   }

   m_entryLabelsModel.setStringList( m_entryLabels);
}

int Functionality::lineForEntryWithLabel(const QString &label)
{
   int index = m_entryLabels.indexOf( label);

   return (index != -1) ? m_entryLines.at(index) : -1;
}


}  // namespace Sequencer
