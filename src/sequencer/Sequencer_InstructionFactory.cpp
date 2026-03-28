#include "Sequencer_InstructionFactory.h"
#include "Sequencer_RepeatInstruction.h"
#include "Sequencer_EndInstruction.h"
#include "Sequencer_PlaylistInstruction.h"
#include "Sequencer_LightInstruction.h"
#include "Sequencer_SequenceInstruction.h"
#include "Sequencer_HallInstruction.h"
#include "Sequencer_EntryInstruction.h"
#include "testableAssert.h"



namespace Sequencer {

InstructionFactory::InstructionFactory( MediaAutomation & audioVideoAutomation_lineA,
                                        MediaAutomation & audioVideoAutomation_lineB,
                                        IF_LightEngineInterface & lightEngine,
                                        OpenWebNetEngine_IF & ownEngine,
                                        const OwnModel & ownModel,
                                        SequenceEditorGui & gui) :
   m_audioVideoAutomation_lineA( audioVideoAutomation_lineA),
   m_audioVideoAutomation_lineB( audioVideoAutomation_lineB),
   m_lightEngine( lightEngine),
   m_ownEngine( ownEngine),
   m_ownModel(ownModel),
   m_gui( gui)
{
}

Instruction *InstructionFactory::buildInstructionForLine(const LineParser &parser, int sourceLineNumber)
{
   Instruction * aInstruction = nullptr;

   switch( parser.getLineKind())
   {
   case LINE_REPEAT:
      aInstruction = buildRepeatInstruction( parser.getLineParamsRepeat(), sourceLineNumber);
      break;

   case LINE_END:
      aInstruction = buildEndInstruction( sourceLineNumber);
      break;

   case LINE_PLAYLIST:
      aInstruction = buildPlaylistInstruction( parser.getLineParamsPlaylist(), sourceLineNumber);
      break;

   case LINE_LIGHT:
      aInstruction = buildLightInstruction( parser.getLineParamsLight(), sourceLineNumber);
      break;

   case LINE_SEQUENCE:
      aInstruction = buildSequenceInstruction( parser.getLineParamsSequence(), sourceLineNumber);
      break;

   case LINE_HALL:
      aInstruction = buildHallInstruction( parser.getLineParamsHall(), m_ownModel, sourceLineNumber);
      break;

   case LINE_ENTRY:
      aInstruction = buildSeqEntryInstruction( parser.getLineParamsEntry(), sourceLineNumber);
      break;

   case LINE_INVALID:
   case LINE_BLANK:
   default:
      break;
   }

   return  aInstruction;
}


Instruction *InstructionFactory::buildRepeatInstruction(const LineParamsRepeat & params, int srcLine)
{
   return new RepeatInstruction( srcLine, params.times, m_gui);
}

Instruction *InstructionFactory::buildEndInstruction(int srcLine)
{
   return new EndInstruction( srcLine);
}

Instruction *InstructionFactory::buildPlaylistInstruction(const LineParamsPlaylist & params, int srcLine)
{
   Instruction * inst =nullptr;

   if (params.line == Playlist::LINE_A)
   {
      inst = new PlaylistInstruction( srcLine, m_audioVideoAutomation_lineA, params.subCommand,
                                      params.label, params.numericParam);
   }
   else if (params.line == Playlist::LINE_B)
   {
      inst = new PlaylistInstruction( srcLine, m_audioVideoAutomation_lineB, params.subCommand,
                                      params.label, params.numericParam);
   }
   else
   {
      T_ASSERT_REP( false, "LINE must be A or B");
   }

   return  inst;
}

Instruction *InstructionFactory::buildLightInstruction(const LineParamsLight & params, int srcLine)
{
   return new LightInstruction( srcLine, m_lightEngine, params.subCommand, params.preset);
}

Instruction *InstructionFactory::buildSequenceInstruction(const LineParamsSequence & params, int srcLine)
{
   return new SequenceInstruction(srcLine, params.subCommand, params.timeSec);
}

Instruction *InstructionFactory::buildHallInstruction(const LineParamsHall & params,
                                                      const OwnModel & ownModel,
                                                      int srcLine)
{
   return new HallInstruction( srcLine, m_ownEngine, params.subCommand,
                               ownModel, params.zone, params.level);
}

Instruction *InstructionFactory::buildSeqEntryInstruction(const LineParamsEntry &params, int srcLine)
{
   return new EntryInstruction( params.label, srcLine);
}

}  // namespace Sequencer
