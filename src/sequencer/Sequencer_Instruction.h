#ifndef SEQUENCER_INTRUCTION_H
#define SEQUENCER_INTRUCTION_H

#include "Sequencer.h"
#include <QObject>

namespace Sequencer {

class InstructionEvent;


/**
 * This is an instruction of the script. All instances
 * have an execution method and a method to retrieve next instruction
 * (that may be dependent on runtime state).
 */
class Instruction
{

public:
   /**
    * @param srcLineNumber is the line number is script source file
    */
   Instruction( int srcLineNumber, LineKind kind = LINE_INVALID);

   virtual ~Instruction() {}

   /** @return the kind of this instruction */
   LineKind instructionKind() const {
      return m_kind;
   }

   /**
    * This function is called by the sript runner. It should be
    * a quick, non blocking function.
    * \param previous is the instruction that was executed before this one.
    *    It can be used, for example, to detect if a loop is being entered
    *    for the first time or it is an iteration. If it is NULL, it should
    *    not be considered.
    */
   virtual void execute( const Instruction * /*previous*/) {
   }

   /**
    * @return the next instruction to be executed by the script. This
    * may depend on runtime status (e.g. for 'repeat' instruction).
    */
   virtual Instruction * next();

   /**
    * set the next instruction for non-jump operations. This is called by the
    * compiler module.
    * @param next may be NULL.
    * @note Function \a next may not return the value set by this function. It
    *   also depends on runtime status.
    */
   virtual void setNext( Instruction * next);

   /**
    * @return a time im ms to wait before executing next instruction.
    * This is 0 for most instructions, with the exeption of 'sequence pause'.
    */
   virtual int delayMs();

   /** @return an event notifier that other modules can attach to.
    * If NULL is returned, users should do nothing. */
   virtual InstructionEvent * eventNotifier() {
      return nullptr;
   }

   /** @return the line number of this instruction in original source code */
   int sourceLineNumber() const;

private:
   int m_srcLineNumber;
   /* next instruction for normal case. Child classes may return a different value */
   Instruction * m_nextInstruction;
   LineKind m_kind;
};

/** helper class to notify events from an instruction to the sequence runner */
class InstructionEvent : public QObject
{
   Q_OBJECT
public:
   InstructionEvent( QObject * parent = nullptr) :
       QObject(parent)
   {
   }

signals:
   void notifyEvent( enum ShowEvents eventId);
};


}  // namespace Sequencer

#endif // SEQUENCER_INTRUCTION_H
