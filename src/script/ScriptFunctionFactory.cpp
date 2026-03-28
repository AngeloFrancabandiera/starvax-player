#include "ScriptFunctionFactory.h"

#include "Action.h"

#include "ScriptGuiFactory.h"
#include "ScriptEngine.h"
#include "QtTimerService.h"
#include "ScriptActionTrigger.h"
#include "ApplicationIcon.h"
#include "testableAssert.h"


/**
 * @class ScriptFunctionFactory
 * This factory builds an HTML viewer with all optimizations useful
 * for script management, (automatic scroll, light and media action
 * recognition, launching and editing, etc...).
 *
 * Note that the engine requires the output from GUI factory, and this
 * is because engine operation depends on the content of text viewer,
 * that in Qt framework is a GUI
 */

ScriptFunctionFactory::ScriptFunctionFactory(QAction *editModeAction, QObject *parent) :
   QObject(parent),
   m_editModeAction( editModeAction)
{
}

ScriptEngine *ScriptFunctionFactory::build( QWidget *container,
                                            QAbstractListModel *mediaModelLineA,
                                            QAbstractListModel *mediaModelLineB,
                                            QStringListModel & sequenceEntryModel,
                                            StatusDisplay * msgDisplay,
                                            ApplicationSettings & settings,
                                            QAbstractListModel *lightModel)
{
   T_ASSERT( m_editModeAction);

   ScriptActionTrigger *actionTrigger = new ScriptActionTrigger( this);

   ScriptGuiFactory *guiFactory = new ScriptGuiFactory( container);
   IF_ScriptViewerInterface *viewer = guiFactory->build( container, actionTrigger,
                                                         mediaModelLineA, mediaModelLineB, sequenceEntryModel,
                                                         msgDisplay, settings, lightModel);

   QtTimerService *timerService = new QtTimerService( this);

   ScriptEngine *engine = new ScriptEngine( *viewer, *timerService, this);
   QList<QAction *> menuActions = createEngineActions( engine);

   connect( engine, & ScriptEngine::addLightEventRequest, viewer, & IF_ScriptViewerInterface::addLightEvent );
   connect( engine, & ScriptEngine::addMediaEventRequest, viewer, & IF_ScriptViewerInterface::addMediaEvent );
   connect( engine, & ScriptEngine::addSeqEntryEventRequest, viewer, & IF_ScriptViewerInterface::addSeqEntryEvent );
   connect( engine, & ScriptEngine::findTextRequest, viewer, & IF_ScriptViewerInterface::activateSearchBar );
   connect( engine, & ScriptEngine::markAsCharacterRequest,
            viewer, & IF_ScriptViewerInterface::markWordUnderCursorAsCharacter );

   viewer->setMenuActionList( menuActions);

   connect( viewer, & IF_ScriptViewerInterface::textChanged,
            engine, & ScriptEngine::textChanged );
   connect( viewer, & IF_ScriptViewerInterface::loadShowRequest,
            engine, & ScriptEngine::loadShowRequest);

   connect( actionTrigger, & ScriptActionTrigger::activateLight,
            engine, & ScriptEngine::activateLight);

   connect( actionTrigger, & ScriptActionTrigger::activateMediaLineA,
            engine, & ScriptEngine::activateMediaLineA );

   connect( actionTrigger, & ScriptActionTrigger::activateMediaLineB,
            engine, & ScriptEngine::activateMediaLineB );

   connect( actionTrigger, & ScriptActionTrigger::activateSequenceEntry,
            engine, & ScriptEngine::activateSequenceEntry);

   connect( engine, & ScriptEngine::newSearchPath,
            viewer, & IF_ScriptViewerInterface::addSearchPath );

   return engine;
}

QList<QAction *> ScriptFunctionFactory::createEngineActions( ScriptEngine *engine)
{
   QList<QAction *> * actionList = new QList<QAction *>();

   Action * scriptPlay = new Action( QIcon(IconPath("script_play.png")),
                                       tr("&play autoscroll"), this);
   scriptPlay->setEnabled( true);
   scriptPlay->setVisibleInModes( false, true);
   connect( scriptPlay, SIGNAL(triggered()), engine, SLOT(playScrollAnimation()) );

   Action * scriptPause = new Action( QIcon(IconPath("script_pause.png")),
                                        tr("&pause autoscroll"), this);
   scriptPause->setEnabled( false);
   scriptPause->setVisibleInModes( false, true);
   connect( scriptPause, SIGNAL(triggered()), engine, SLOT(pauseScrollAnimation()) );

   connect( engine, SIGNAL(scriptIsAnimated(bool)), scriptPlay, SLOT(setDisabled(bool)) );
   connect( engine, SIGNAL(scriptIsAnimated(bool)), scriptPause, SLOT(setEnabled(bool)) );

   Action * findTextAction = new Action( QIcon(IconPath("find.png")), tr("&find text"), this);
   findTextAction->setEnabled( true);
   findTextAction->bindShortcut( QKeySequence(Qt::CTRL | Qt::Key_F));
   connect( findTextAction, SIGNAL(triggered()), engine, SIGNAL(findTextRequest()) );

   Action * scriptAddLightEvent = new Action( QIcon(IconPath("light.png")), tr("add &light event"), this);
   scriptAddLightEvent->setEnabled( false);
   scriptAddLightEvent->setVisibleInModes( true, false);
   connect( scriptAddLightEvent, SIGNAL(triggered()), engine, SIGNAL(addLightEventRequest()) );

   Action * scriptAddMediaEvent = new Action( QIcon(IconPath("sound.png")), tr("add &media event"), this);
   scriptAddMediaEvent->setEnabled( false);
   scriptAddMediaEvent->setVisibleInModes( true, false);
   connect( scriptAddMediaEvent, SIGNAL(triggered()), engine, SIGNAL(addMediaEventRequest()) );

   Action * scriptAddSequenceEntryEvent = new Action( QIcon(IconPath("sequencer_play.png")),
                                                      tr("add &sequence event"), this);
   scriptAddSequenceEntryEvent->setEnabled( false);
   scriptAddSequenceEntryEvent->setVisibleInModes( true, false);
   connect( scriptAddSequenceEntryEvent, SIGNAL(triggered()), engine, SIGNAL(addSeqEntryEventRequest()) );

   Action * markAsCharacter = new Action( QIcon(IconPath("charcter.png")), tr("mark as &character"), this);
   markAsCharacter->setEnabled( false);
   markAsCharacter->setVisibleInModes( true, false);
   connect( markAsCharacter, SIGNAL(triggered()), engine, SIGNAL(markAsCharacterRequest()) );

   *actionList << findTextAction << scriptPlay << scriptPause << scriptAddMediaEvent
              << scriptAddLightEvent << scriptAddSequenceEntryEvent << markAsCharacter;

   connect( m_editModeAction, SIGNAL(triggered(bool)), scriptAddLightEvent, SLOT(setEnabled(bool)));
   connect( m_editModeAction, SIGNAL(triggered(bool)), scriptAddMediaEvent, SLOT(setEnabled(bool)));
   connect( m_editModeAction, SIGNAL(triggered(bool)), scriptAddSequenceEntryEvent, SLOT(setEnabled(bool)));
   connect( m_editModeAction, SIGNAL(triggered(bool)), markAsCharacter, SLOT(setEnabled(bool)));

   connect( m_editModeAction, SIGNAL(triggered(bool)), scriptPlay, SLOT(setDisabled(bool)));

   engine->setActionList( actionList);

   return *actionList;
}
