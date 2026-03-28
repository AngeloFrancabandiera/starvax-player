#include "Sequencer_Runner.h"
#include "Sequencer_Instruction.h"

#include "ITimerService.h"
#include "SequenceEditorGui.h"

#include "testableAssert.h"


namespace Sequencer {

Runner::Runner( QList<Instruction *> & sequence,
                ITimerService & timerService,
                SequenceEditorGui & gui,
                QObject *parent) :
   QObject ( parent),
   m_sequence( sequence),
   m_timer( timerService),
   m_gui( gui),
   m_current( nullptr),
   m_previous( nullptr),
   m_callback( new TimeoutListener(this))
{
   refreshLineLookupTable();
   connect( m_callback, SIGNAL(timeout()), this, SLOT(onTimeout()));
}


void Runner::resume()
{
   if (m_current != nullptr)
   {
      m_gui.setActiveLine( m_current->sourceLineNumber());

      if (m_current->eventNotifier() != nullptr)
      {
         connect( m_current->eventNotifier(), & InstructionEvent::notifyEvent,
                  this, & Runner::onInstructionEvent);
      }

      m_current->execute( m_previous);

      int delayMs = m_current->delayMs();

      if (delayMs >= 0)
      {
         armNextInstruction(delayMs);
      }
      else
      {
         handleNextInstructionMode( delayMs);
      }
   }
   else
   {
      m_gui.setRunningMode( false);
   }
}

void Sequencer::Runner::armNextInstruction(int delayMs)
{
   T_ASSERT(m_current != nullptr);

   if (m_current->eventNotifier() != nullptr)
   {
      disconnect( m_current->eventNotifier(), & InstructionEvent::notifyEvent,
                  this, & Runner::onInstructionEvent);
   }

   m_previous = m_current;
   m_current = m_current->next();

   /* wait for delay specified by this instruction */
   m_timer.triggerTimeout( delayMs, m_callback);
   m_gui.setRunningMode( true);
}


int Runner::setCurrentInstructionByLine( int line)
{
   // TBD what happens if 'current' is changed but not retriggered immediately?
   stop();

   /* set new instruction */
   m_current = m_instructionForLine.value( line, nullptr);

   if (m_current == nullptr)
   {
      /* point to first instruction line following 'line' */
      searchClosestValidLine( line);
   }

   /* an instruction has been manually activated, so don't pass
    * a previous instruction for the execution */
   m_previous = nullptr;

   return (m_current != nullptr) ? m_current->sourceLineNumber() : 1;
}

void Runner::stop()
{
   if ((m_current != nullptr) && (m_current->eventNotifier() != nullptr))
   {
      disconnect( m_current->eventNotifier(), & InstructionEvent::notifyEvent,
                  this, & Runner::onInstructionEvent);
   }

   m_timer.stopTimer();
   m_gui.setRunningMode( false);
}

void Runner::onScriptRebuilt()
{
   refreshLineLookupTable();
}

void Runner::onTimeout()
{
   m_timer.stopTimer();
   resume();
}

void Runner::onInstructionEvent(ShowEvents eventId)
{
   if (eventId == Media_trackFinished)
   {
      armNextInstruction(0);
   }
}


void Runner::refreshLineLookupTable()
{
   m_instructionForLine.clear();

   foreach (Instruction * instr, m_sequence)
   {
      if (instr != nullptr)
      {
         m_instructionForLine.insert( instr->sourceLineNumber(), instr);
      }
   }
}

void Runner::searchClosestValidLine( int line)
{
   /* search in the keys of 'm_lineForInstruction' map the lowest
    * value that greater or equal to 'line'. Assume that QMap sorts
    * key in ascending order */
   bool found = false;

   QMap<int, Instruction *>::Iterator i;

   for (i = m_instructionForLine.begin(); (i != m_instructionForLine.end()) && ( ! found); i++)
   {
      if (i.key() >= line)
      {
         m_current = i.value();
         found = true;
      }
   }

   if ((found == false) && ( ! m_instructionForLine.isEmpty()))
   {
      m_current = m_instructionForLine.last();
   }
}

/**
 * @brief handle how to wait for next instruction, in case the
 *    "delay" associated with current instruction is negative.
 * @param mode is returned by "delayMs" of current instruction
 */
void Runner::handleNextInstructionMode(int mode)
{
    switch(mode)
    {
    case Sequencer::DELAY_FOREVER:
        /* the sequencer has been stopped. Update GUI accordingly */
        m_gui.setRunningMode( false);
        break;

    case Sequencer::DELAY_UNTIL_EVENT:
        /* Do nothing. An event will arrive */
        break;

    default:
        T_ASSERT( false);
    }
}



}  // namespace Sequencer

