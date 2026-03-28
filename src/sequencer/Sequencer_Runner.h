#ifndef SEQUENCER_RUNNER_H
#define SEQUENCER_RUNNER_H

#include <QList>
#include <QMap>
#include <QObject>
#include <Sequencer.h>

class TimeoutListener;
class ITimerService;
class SequenceEditorGui;

namespace Sequencer {

class Instruction;


/**
 * This class runs a sequence of instruction that have
 * already been ordered (by setting field 'next' of each one).
 */
class Runner : public QObject
{
   Q_OBJECT
public:
   Runner( QList<Instruction *> & sequence,
           ITimerService &timerService,
           SequenceEditorGui & gui,
           QObject * parent = nullptr);

   /** resume the execution of a sequence.
    * From the current instruction, the loop is:
    * - call execute method
    * - ask the next instruction and set it as current
    * - wait a delay specified by executed instruction
    */
   void resume();

   /**
    * specify a line in source code to set relevant instruction as active.
    * If no instruction has the specified line number, the current instruction
    * is the one with smallest line number that is greater \p line.
    * If no valid instruction has line number greater or equal to line, the last
    * valid instruction is active.
    * @param line
    * @return the line number that has been actually activated. This is equal
    *   to \p line if such line has a valid instruction
    */
   int setCurrentInstructionByLine( int line);

   /** stop the execution of the sequence.
    * \note: this does not stop playback of media or
    *        transition of light preset
    */
   void stop();

   /**
    * this function must be called whenever the instruction sequence
    * changes (or may have changed for a rebuild).
    */
   void onScriptRebuilt();

private slots:
   /** timer callback. This is used to avoid a unique loop: each
    * instruction is executed as a timeout after previous instruction.
    * (Such timeout may be 0)
    */
   void onTimeout();

   /**
    * called when current instruction catches an event that makes
    * the sequence evolve
    */
   void onInstructionEvent( enum ShowEvents eventId);

private:
   QList<Instruction *> & m_sequence;
   ITimerService & m_timer;
   SequenceEditorGui & m_gui;
   Instruction * m_current;
   Instruction * m_previous;
   QMap<int, Instruction *> m_instructionForLine;

   TimeoutListener * m_callback;

private:
   void refreshLineLookupTable();
   void searchClosestValidLine( int line);
   void handleNextInstructionMode( int mode);
   void armNextInstruction(int delayMs);
};


}  // namespace Sequencer

#endif // SEQUENCER_RUNNER_H
