#include <SequencerFactory.h>
#include <QAction>

#include "SequenceEditorGuiQt.h"
#include "Sequencer_Instruction.h"
#include "Sequencer_InstructionFactory.h"
#include "Sequencer_Functionality.h"
#include "Sequencer_ScriptSequencer.h"
#include "Sequencer_Runner.h"
#include "Sequencer_ScriptParser.h"
#include "Sequencer_LineParser.h"
#include "QtTimerService.h"
#include "ApplicationIcon.h"

namespace Sequencer {


SequenceEditorGui *Sequencer::SequencerFactory::buildGuiHandler( StatusDisplay & msgDisplay,
                                                                 const ApplicationSettings & settings,
                                                                 QWidget * parent)
{
   return new SequenceEditorGuiQt( msgDisplay, settings, parent);
}

InstructionFactory *SequencerFactory::buildInstructionFactory( MediaAutomation & audioVideoAutomation_lineA,
                                                               MediaAutomation & audioVideoAutomation_lineB,
                                                               IF_LightEngineInterface & lightEngine,
                                                               OpenWebNetEngine_IF & ownEngine,
                                                               const OwnModel & ownModel,
                                                               SequenceEditorGui & gui)
{
   return new Sequencer::InstructionFactory( audioVideoAutomation_lineA, audioVideoAutomation_lineB,
                                             lightEngine, ownEngine, ownModel, gui);
}

Functionality *SequencerFactory::buildFunctionality( SequenceEditorGui & gui,
                                                     InstructionFactory & instructionFactory,
                                                     QObject *parent)
{
   Sequencer::ScriptSequencer * sequencer = new Sequencer::ScriptSequencer();
   QtTimerService * timer = new QtTimerService(parent);
   Sequencer::LineParser * lineParser = new Sequencer::LineParser();

   Sequencer::ScriptParser * scriptParser = new Sequencer::ScriptParser( *lineParser, instructionFactory );
   Sequencer::Runner * runner = new Sequencer::Runner( scriptParser->instructionSequence(), *timer,
                                                       gui, parent);
   Sequencer::Functionality * function =  new Sequencer::Functionality( gui, *sequencer, *runner,
                                                                        *scriptParser, parent);

   return  function;
}

QList<QAction *> SequencerFactory::buildActions(Functionality * sequencer)
{
   QAction * resumeAction = new QAction( QIcon(IconPath("sequencer_play.png")),
                                         QObject::tr("resume"), sequencer);
   resumeAction->setShortcut( Qt::Key_F5);
   QObject::connect( resumeAction, & QAction::triggered, sequencer, & Functionality::resume);

   QAction * pauseAction = new QAction( QIcon(IconPath("sequencer_pause.png")),
                                        QObject::tr("stop"), sequencer);
   pauseAction->setShortcut( Qt::Key_F6);
   QObject::connect( pauseAction, & QAction::triggered, sequencer, & Functionality::stop);

   return QList<QAction *>() << resumeAction << pauseAction;
}

}  // namespace Sequencer
