#ifndef SEQUENCER_HALLINSTRUCTION_H
#define SEQUENCER_HALLINSTRUCTION_H

#include <QString>
#include "Sequencer_Instruction.h"

class OpenWebNetEngine_IF;
class OwnModel;

namespace Sequencer {

class HallInstruction : public Instruction
{
public:
   /**
    * @param srcLineNumber
    * @param ownEngine
    * @param command
    * @param zoneName is the readable name in OWN table.
    *     It will be transalted in OWN notation when instruction is executed.
    * @param level
    */
   HallInstruction( int srcLineNumber,
                    OpenWebNetEngine_IF & ownEngine,
                    HallSubCommand command,
                    const OwnModel & model,
                    QString zoneName = QString(),
                    int level = 0);

   // Instruction interface
public:
   void execute(const Instruction *) override;

private:
   OpenWebNetEngine_IF & m_ownEngine;
   HallSubCommand m_command;
   const OwnModel & m_model;
   QString m_zoneName;
   int m_level;
   QString m_zoneTag;

private:
   void turnOnCommand();
};

}  // namespace Sequencer

#endif // SEQUENCER_HALLINSTRUCTION_H
