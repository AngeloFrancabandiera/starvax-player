#ifndef SEQUENCER_INSTRUCTIONFACTORY_H
#define SEQUENCER_INSTRUCTIONFACTORY_H

#include <array>

#include "Sequencer_InstructionFactory_IF.h"

class MediaAutomation;
class IF_LightEngineInterface;
class OpenWebNetEngine_IF;
class SequenceEditorGui;
class OwnModel;

namespace Sequencer {

class Instruction;


class InstructionFactory : public InstructionFactory_IF
{
protected:
   ~InstructionFactory() override {}

public:
   InstructionFactory( std::array<MediaAutomation *, NUMBER_OF_MEDIA_DECKS> & mediaAutomationSet,
                       IF_LightEngineInterface & lightEngine,
                       OpenWebNetEngine_IF & ownEngine,
                       const OwnModel &ownModel,
                       SequenceEditorGui & gui);

   // InstructionFactory_IF interface
   Instruction *buildInstructionForLine( const LineParser & parser, int sourceLineNumber) override;

private:
   Instruction *buildRepeatInstruction( const LineParamsRepeat &params, int srcLine);
   Instruction *buildEndInstruction( int srcLine);
   Instruction *buildPlaylistInstruction( const LineParamsPlaylist &params, int srcLine);
   Instruction *buildLightInstruction( const LineParamsLight &params, int srcLine);
   Instruction *buildSequenceInstruction(const LineParamsSequence &params, int srcLine);
   Instruction *buildHallInstruction( const LineParamsHall &params, const OwnModel & ownModel,
                                      int srcLine);
   Instruction *buildSeqEntryInstruction( const LineParamsEntry &params, int srcLine);

private:
   std::array<MediaAutomation *, NUMBER_OF_MEDIA_DECKS> & m_audioVideoAutomationSet;
   IF_LightEngineInterface & m_lightEngine;
   OpenWebNetEngine_IF & m_ownEngine;
   const OwnModel & m_ownModel;
   SequenceEditorGui & m_gui;
};

}  // namespace Sequencer

#endif // SEQUENCER_INSTRUCTIONFACTORY_H
