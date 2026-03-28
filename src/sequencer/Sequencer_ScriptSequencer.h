#ifndef SEQUENCER_SCRIPTSEQUENCER_H
#define SEQUENCER_SCRIPTSEQUENCER_H

#include <QList>
#include <QStack>

namespace Sequencer {

class Instruction;


class ScriptSequencer
{
public:
   ScriptSequencer();

   struct Issue
   {
      Instruction * refInstruction;
      QString issueDescription;

      Issue( Instruction * aInstruction,
             QString aIssueDescription) :
         refInstruction( aInstruction),
         issueDescription( aIssueDescription)
      {}
   };

   /**
    * check the labels for the entries and find duplicates.
    * In case of duplicates, an issue is filled.
    * @param instructions is a list of instructions in the order of
    *    the source file.
    * @return TRUE if no issue is found
    */
   bool checkEntries( const QList<Instruction *> & instructions);

   /**
    * analyse a list of instructions and fill 'next' fields of
    * each one to build up the sequence.
    * @param instructions is a list of instructions in the order of
    *    the source file.
    * @return TRUE if process completes without issues.
    */
   bool makeSequenceInstructionList(QList<Instruction *> & instructions);

   /**
    * @return the issues that were found during the last call to
    * \a ::makeSequenceInstructionList
    */
   const QList<Issue *> issues() const {
      return m_issues;
   }

private:
   /* stack of instructions that require an end for flow control,
    * (as far as now, only 'repeat') */
   QStack<Instruction *> m_endRequiredStack;
   QList<Issue *> m_issues;
};

}  // namespace Sequencer

#endif // SEQUENCER_SCRIPTSEQUENCER_H
