#ifndef SEQUENCERFACTORY_H
#define SEQUENCERFACTORY_H

#include <QList>
#include <array>
class QAction;

class SequenceEditorGui;
class QWidget;
class QObject;
class MediaAutomation;
class IF_LightEngineInterface;
class OpenWebNetEngine_IF;
class OwnModel;
class StatusDisplay;
class ApplicationSettings;

namespace Sequencer {
class Functionality;
class ScriptSequencer;
class Runner;
class ScriptParser;
class InstructionFactory;



class SequencerFactory
{
public:
   SequencerFactory() {}

   SequenceEditorGui * buildGuiHandler( StatusDisplay & msgDisplay,
                                        const ApplicationSettings &settings,
                                        QWidget *parent);

   InstructionFactory * buildInstructionFactory( std::array<MediaAutomation *, NUMBER_OF_MEDIA_DECKS> & mediaAutomationSet,
                                                 IF_LightEngineInterface & lightEngine,
                                                 OpenWebNetEngine_IF & ownEngine,
                                                 const OwnModel &ownModel,
                                                 SequenceEditorGui & gui);

   Functionality * buildFunctionality(SequenceEditorGui & gui, InstructionFactory & instructionFactory,
                                       QObject * parent = nullptr);

   QList<QAction *> buildActions( Sequencer::Functionality * sequencer);
};

}  // namespace Sequencer

#endif // SEQUENCERFACTORY_H
