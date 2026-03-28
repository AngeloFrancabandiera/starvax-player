#include "Sequencer_LightInstruction.h"

namespace Sequencer {


LightInstruction::LightInstruction( int srcLineNumber,
                                    IF_LightEngineInterface &lightEngine,
                                    Sequencer::LightSubCommand command,
                                    const QString & preset) :
   Instruction( srcLineNumber),
   m_lightEngine( lightEngine),
   m_command( command),
   m_preset( preset)
{
}


void Sequencer::LightInstruction::execute(const Sequencer::Instruction * )
{
   switch (m_command)
   {
   default:
   case LIGHT_NONE:
      break;

   case LIGHT_TRIGGER:
      m_lightEngine.triggerTransition( m_preset);
      break;

   case LIGHT_STOP:
      m_lightEngine.freezeTransition();
      break;
   }
}


}  // namespace Sequencer

