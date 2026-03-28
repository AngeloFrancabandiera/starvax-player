#include "Sequencer_ScriptSequencer.h"
#include "Sequencer_Instruction.h"
#include "Sequencer_RepeatInstruction.h"
#include "Sequencer_EntryInstruction.h"
#include "testableAssert.h"

#include <QObject>


namespace Sequencer {

ScriptSequencer::ScriptSequencer()
{
}

bool ScriptSequencer::checkEntries(const QList<Instruction*>& instructions)
{
   QStringList labels;
   bool ok = true;

   m_issues.clear();

   for (Instruction * i : instructions)
   {
      if (i->instructionKind() == LINE_ENTRY)
      {
         QString label = dynamic_cast<EntryInstruction *>(i)->label();

         if (labels.contains( label))
         {
            ok = false;
            Issue * aIssue = new Issue( i, QObject::tr("label %1 is repeated").arg(label));
            m_issues << aIssue;
         }
         else
         {
            labels << label;
         }
      }
   }

   return ok;
}



bool ScriptSequencer::makeSequenceInstructionList( QList<Instruction *> & instructions)
{
   int size = instructions.size();
   m_endRequiredStack.clear();
   m_issues.clear();

   for (int i = 0; i < size; ++i)
   {
      /* store current and following instruction, as they are in source code */
      Instruction * current = instructions.at(i);
      Instruction * following = (i < (size - 1)) ? instructions.at(i+1) : nullptr;

      T_ASSERT(current != nullptr);

      if (current->instructionKind() == LINE_END)
      {
         /* 'next' must point to relevant 'repeat' */
         if (m_endRequiredStack.isEmpty() == false)
         {
            Instruction * opening = m_endRequiredStack.pop();
            T_ASSERT( opening != nullptr);
            T_ASSERT( opening->instructionKind() == LINE_REPEAT);

            current->setNext( opening);

            /* instruction to execute after loop */
            dynamic_cast<RepeatInstruction *>(opening)->setInstructionOutsideLoop( following);
         }
         else
         {
            /* we've found a bug in source code */
            Issue * err = new Issue( current, QObject::tr("<b>end</b> does not close a <b>repeat</b> line"));
            m_issues << err;
         }
      }
      else if (current->instructionKind() == LINE_REPEAT)
      {
         /* 'next' as others, but a 'post loop' instruction must be set */
         m_endRequiredStack.push( current);
         current->setNext( following);
      }
      else
      {
         /* default: 'next' is simply the following instruction */
         current->setNext( following);
      }
   }

   /* check if all instructions requiring an 'end' are ended */
   foreach (Instruction * instr, m_endRequiredStack)
   {
      T_ASSERT( instr != nullptr);
      Issue * err = new Issue( instr,
                               QObject::tr("instruction at line %1 missing <b>end</b>").
                               arg(instr->sourceLineNumber()));
      m_issues << err;
   }


   return (m_issues.count() == 0);
}



}  // namespace Sequencer
