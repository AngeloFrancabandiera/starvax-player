#include "Sequencer_HallInstruction.h"

#include "OpenWebNetEngine_IF.h"
#include "OwnModel.h"

namespace Sequencer {

HallInstruction::HallInstruction( int srcLineNumber,
                                  OpenWebNetEngine_IF & ownEngine,
                                  HallSubCommand command,
                                  const OwnModel &model,
                                  QString zoneName,
                                  int level) :
   Instruction( srcLineNumber),
   m_ownEngine( ownEngine),
   m_command( command),
   m_model( model),
   m_zoneName( zoneName),
   m_level( level)
{
}

void HallInstruction::execute(const Sequencer::Instruction *)
{
   m_zoneTag = m_model.tagForName( m_zoneName);

   switch (m_command)
   {
   default:
   case HALL_NONE:
      break;

   case HALL_ON:
      turnOnCommand();
      break;

   case HALL_OFF:
      m_ownEngine.turnOff( m_zoneTag);
      break;
   }
}

void HallInstruction::turnOnCommand()
{
   if (m_level == HALL_TRUN_ON_FULL)
   {
      m_ownEngine.turnOn( m_zoneTag);
   }
   else
   {
      m_ownEngine.dimmer( m_level, m_zoneTag);
   }
}


}  // namespace Sequencer

