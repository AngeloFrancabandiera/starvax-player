#ifndef SEQUENCER_LIGHTINSTRUCTION_H
#define SEQUENCER_LIGHTINSTRUCTION_H

#include <Sequencer_Instruction.h>
#include "IF_LightEngineInterface.h"

namespace Sequencer {


class LightInstruction : public Instruction
{
public:
   LightInstruction( int srcLineNumber,
                     IF_LightEngineInterface & lightEngine,
                     LightSubCommand command,
                     const QString & preset = QString());

   // Instruction interface
public:
   void execute(const Instruction * previous = nullptr) override;

private:
   IF_LightEngineInterface & m_lightEngine;
   LightSubCommand m_command;
   QString m_preset;
};

}  // namespace Sequencer

#endif // SEQUENCER_LIGHTINSTRUCTION_H
