#include "ApplicationFactory.h"
#include <QAction>
#include <QVideoWidget>

#include "Action.h"
#include "LightPresetModel.h"
#include "MediaListModel.h"
#include "SequencerFactory.h"
#include "SequenceEditorGui.h"

#include "FileInport.h"
#include "ApplicationSettings.h"
#include "ApplicationIcon.h"
#include "ShowManager.h"
#include "ShowFactory.h"
#include "ShowFileInfo.h"
#include "ScriptFunctionFactory.h"
#include "ScriptEngine.h"

#include "PlaylistFunctionFactory.h"
#include "PlaylistGuiFactory.h"
#include "PlaylistBar.h"
#include "ActionListController.h"
#include "ActionListView.h"
#include "volumeslider.h"
#include "ExponentialFader.h"
#include "MediaListModel.h"
#include "MediaAutomation.h"
#include "PicturePlaybar.h"
#include "FullScreenMediaWidget.h"
#include "StillPictureWidget.h"
#include "PlaylistConstants.h"

#include "LightFunctionFactory.h"
#include "LightPresetFactory.h"
#include "lightControlGuiFactory.h"
#include "IF_LightEngineInterface.h"
#include "IF_DmxSliderPanel.h"
#include "IF_GuiLightControlPanel.h"

#include "mainwindow.h"
#include "AppSettingsGui.h"

#include "ServerFactory.h"
#include "ServerEngine.h"
#include "NetworkInterface.h"
#include "ActionModeFilter.h"
#include "StatusDisplayQt.h"

#include "DmxInterfaceFactory.h"

#include "OpenWebNetFactory.h"
#include "OpenWebNetEngine_IF.h"
#include "OpenWebNetGUIFactory.h"
#include "OpenWebNetPanel.h"
#include "OwnModel.h"
#include "OwnFileLoader.h"

#include "Sequencer_Functionality.h"

#include <QFontDatabase>


ApplicationFactory::ApplicationFactory()
{
   ApplicationSettings *applicationSettings = new ApplicationSettings( this);
   FileInport *fileInport = new FileInport( *applicationSettings, this);
   DmxInterfaceFactory dmxFactory;
   DmxInterface * dmxInterface= dmxFactory.build();
   ActionModeFilter * actionMode = new ActionModeFilter( this);

   /* main window */
   m_mainWindow = new MainWindow( *fileInport, *applicationSettings, nullptr);

   AppSettingsGui * appSettingsGui = new AppSettingsGui( *applicationSettings, m_mainWindow);
   m_mainWindow->attachSettingsGui( appSettingsGui);

   /* edit mode for Show action. Don't add it to 'actionMode' */
   Action * setEditModeAction = new Action( QIcon(IconPath("script_edit.png")),
                                            tr("&set edit mode"), this);
   setEditModeAction->bindShortcut( QString("Ctrl+E"));
   setEditModeAction->setVisibleInModes( true, true);
   setEditModeAction->setCheckable( true);
   setEditModeAction->setChecked( false);

   /* this should be the first slot to be connected */
   connect( setEditModeAction, SIGNAL(triggered(bool)), m_mainWindow, SLOT(setEditMode(bool)) );
   m_mainWindow->addShowActions( QList<QAction *>() << setEditModeAction);

   /* common mean to show messages to user */
   StatusDisplay * statusDisplay = new StatusDisplayQt( *mainWindow()->statusBar());

   /* light control function */
   LightFunctionFactory *lightFactory = new LightFunctionFactory( this);
   LightPresetFactory *lightPresetFactory = new LightPresetFactory( this);
   int numberOfDmxChannels = applicationSettings->numberOfDmxChannels();

   IF_LightTransitionEngine *transitionEngine = lightFactory->buildTransitionEngine( numberOfDmxChannels);
   LightPresetModel *lightModel = lightFactory->buildModel( numberOfDmxChannels,
                                                            *lightPresetFactory);
   ActionListController *lightViewController = new ActionListController( *lightModel, this);
   IF_LightEngineInterface *lightEngine = lightFactory->buildLightEngine( *lightModel, *transitionEngine,
                                                                          *lightViewController,
                                                                          *statusDisplay,
                                                                          *dmxInterface);

   LightControlGuiFactory *lightGuiFactory = new LightControlGuiFactory( numberOfDmxChannels, *applicationSettings,
                                                                         m_mainWindow);


   IF_DmxSliderPanel *sliderPanel = lightGuiFactory->buildDmxSliderPanel( lightEngine, transitionEngine,
                                                                          applicationSettings, m_mainWindow);
   IF_GuiLightControlPanel *lightPresetPanel;
   lightPresetPanel = lightGuiFactory->buildPresetListViewer( lightEngine, lightModel,
                                                              sliderPanel, lightViewController,
                                                              statusDisplay, m_mainWindow);

   connect( m_mainWindow, SIGNAL(mainWindowAboutToClose()), lightEngine, SLOT(requestDmxStop()) );

   lightGuiFactory->buildLightControlPanel( lightEngine, lightModel, sliderPanel, lightPresetPanel,
                                            setEditModeAction, m_mainWindow->lightControlArea());
   QList<QAction *> lightActions = lightGuiFactory->buildActions( lightPresetPanel);
   m_mainWindow->addLightControlActions( lightActions);
   actionMode->addActions( lightActions);

   connect( sliderPanel, SIGNAL(manualOnlyChanged(bool,int)),
            lightPresetPanel, SLOT(onManualOnlyChangedByGUI(bool,int)) );
   connect( sliderPanel, SIGNAL(createPresetFromValues(QList<double>)),
            lightModel, SLOT(createPresetWithValues(QList<double>)) );

   /* playlist function */
   PlaylistFunctionFactory *playlistFactory;
   playlistFactory = new PlaylistFunctionFactory( this);

   FullScreenMediaWidget * mediaWidget_A = new FullScreenMediaWidget( m_mainWindow);

   FullScreenMediaWidget * mediaWidget_B = new FullScreenMediaWidget( m_mainWindow);

   ExponentialFader *expFader_A = new ExponentialFader( this);
   MediaListModel *playlistModel_A = playlistFactory->buildModel("PA");
   IF_MediaEngineInterface * mediaEngine_A = playlistFactory->buildMediaEngine( *expFader_A,
                                                                                *mediaWidget_A,
                                                                                *statusDisplay,
                                                                                applicationSettings);
   mediaEngine_A->setVolume( applicationSettings->defaultVolumeLineA());

   ExponentialFader *expFader_B = new ExponentialFader( this);
   MediaListModel *playlistModel_B = playlistFactory->buildModel("PB");
   IF_MediaEngineInterface * mediaEngine_B = playlistFactory->buildMediaEngine( *expFader_B,
                                                                                *mediaWidget_B,
                                                                                *statusDisplay,
                                                                                applicationSettings);
   mediaEngine_B->setVolume( applicationSettings->defaultVolumeLineB());

   ActionListController * mediaActionController_A = new ActionListController( *playlistModel_A, this);

   MediaAutomation *audioVideoAutomation_A =
         playlistFactory->buildAutomation( playlistModel_A, mediaEngine_A,
                                           mediaActionController_A, expFader_A,
                                           applicationSettings->defaultVolumeLineA(),
                                           *statusDisplay);

   PlaylistGuiFactory *playGuiFactory = new PlaylistGuiFactory( *applicationSettings, this);
   AudioVideoPlayBar *audvidPlaybar_A = playGuiFactory->buildAudioVideoPlaybar( mediaEngine_A, m_mainWindow);
   PicturePlaybar * pictPlaybar_A = playGuiFactory->buildPicturePlaybar( mediaEngine_A, *audioVideoAutomation_A, m_mainWindow);
   PlaylistBar * playlistBar_A = playGuiFactory->buildPlaylistBar( audvidPlaybar_A, pictPlaybar_A,
                                                                   playlistModel_A, m_mainWindow);

   ActionListView *playlistView_A = playGuiFactory->buildPlaylistView( playlistBar_A, playlistModel_A,
                                                                       mediaActionController_A,
                                                                       statusDisplay,
                                                                       m_mainWindow);
   pictPlaybar_A->setThumbnailContainer( playlistView_A);
   playlistView_A->setDragEnabled(true);
   playlistView_A->setTitle(tr("Playlist Line A"));

   audioVideoAutomation_A->activateNextAfterPlaybck( applicationSettings->activateNextAfterPLayback());
   connect( applicationSettings, & ApplicationSettings::activateNextAfterPLaybackChanged,
            audioVideoAutomation_A, & MediaAutomation::activateNextAfterPlaybck);

   ActionListController * mediaActionController_B = new ActionListController( *playlistModel_B, this);

   MediaAutomation *audioVideoAutomation_B =
         playlistFactory->buildAutomation( playlistModel_B, mediaEngine_B,
                                           mediaActionController_B, expFader_B,
                                           applicationSettings->defaultVolumeLineB(),
                                           *statusDisplay);

   AudioVideoPlayBar *audvidPlaybar_B = playGuiFactory->buildAudioVideoPlaybar( mediaEngine_B, m_mainWindow);
   PicturePlaybar * pictPlaybar_B = playGuiFactory->buildPicturePlaybar( mediaEngine_B, *audioVideoAutomation_B,
                                                                         m_mainWindow);
   PlaylistBar * playlistBar_B = playGuiFactory->buildPlaylistBar( audvidPlaybar_B, pictPlaybar_B,
                                                                   playlistModel_B, m_mainWindow);

   ActionListView *playlistView_B = playGuiFactory->buildPlaylistView( playlistBar_B, playlistModel_B,
                                                                       mediaActionController_B,
                                                                       statusDisplay,
                                                                       m_mainWindow);

   connect( mediaActionController_A, & ActionListController::requestToPlay,
            mediaEngine_A, & IF_MediaEngineInterface::play);
   connect( mediaActionController_B, & ActionListController::requestToPlay,
            mediaEngine_B, & IF_MediaEngineInterface::play);

   pictPlaybar_B->setThumbnailContainer( playlistView_B);
   playlistView_B->setDragEnabled(true);
   playlistView_B->setTitle(tr("Playlist Line B"));

   audioVideoAutomation_B->activateNextAfterPlaybck( applicationSettings->activateNextAfterPLayback());
   connect( applicationSettings, & ApplicationSettings::activateNextAfterPLaybackChanged,
            audioVideoAutomation_B, & MediaAutomation::activateNextAfterPlaybck);

   playGuiFactory->buildPlaylistPanel( mediaEngine_A, audioVideoAutomation_A, playlistModel_A,
                                       Playlist::LINE_A, playlistView_A, fileInport,
                                       setEditModeAction, m_mainWindow->playlistAreaLineA());

   playGuiFactory->buildPlaylistPanel( mediaEngine_B, audioVideoAutomation_B, playlistModel_B,
                                       Playlist::LINE_B, playlistView_B, fileInport,
                                       setEditModeAction,  m_mainWindow->playlistAreaLineB());

   playGuiFactory->buildVolumeBar( mediaEngine_A, m_mainWindow->volumeSliderAreaLineA() );
   playGuiFactory->buildVolumeBar( mediaEngine_B, m_mainWindow->volumeSliderAreaLineB() );

   connect( applicationSettings, SIGNAL(defaultVolumeChangedLineA(int)),
            audioVideoAutomation_A, SLOT(setDefaultVolume(int)) );
   connect( applicationSettings, SIGNAL(defaultVolumeChangedLineB(int)),
            audioVideoAutomation_B, SLOT(setDefaultVolume(int)) );

   connect( m_mainWindow, SIGNAL(mainWindowAboutToClose()), audioVideoAutomation_A, SLOT(onClosing()) );
   connect( m_mainWindow, SIGNAL(mainWindowAboutToClose()), audioVideoAutomation_B, SLOT(onClosing()) );

   connect( mediaActionController_A, & ActionListController::activeRowChanged,
            audioVideoAutomation_A, & MediaAutomation::activateByRowNumer);

   connect( mediaActionController_A, & ActionListController::activeRowChanged,
            playlistBar_A, & PlaylistBar::onActiveRowChanged);

   /* build media Actions. Use SHIFT modifier for lINE B */
   QList<QAction *> playlistActionsLineA =
         playlistFactory->buildActionList( mediaEngine_A, audioVideoAutomation_A,
                                           PlaylistFunctionFactory::BaseActions);
   QList<QAction *> playlistActionsLineB =
         playlistFactory->buildActionList( mediaEngine_B, audioVideoAutomation_B,
                                           PlaylistFunctionFactory::ShiftedActions);
   m_mainWindow->addMediaListActions( playlistActionsLineA);
   m_mainWindow->addMediaListActions( playlistActionsLineB);

   actionMode->addActions( playlistActionsLineA);
   actionMode->addActions( playlistActionsLineB);

   /* action for both lines */
   Action * pauseAllAction = new Action( QIcon(IconPath("track_pause_all.png")), tr("pause all"));
   pauseAllAction->bindShortcut(Qt::Key_End);
   pauseAllAction->setCheckable(false);
   connect( pauseAllAction, SIGNAL(triggered()), mediaEngine_A, SLOT(pause()));
   connect( pauseAllAction, SIGNAL(triggered()), mediaEngine_B, SLOT(pause()));

   Action * playAllAction = new Action( QIcon(IconPath("track_play_all.png")), tr("play all"));
   playAllAction->bindShortcut(Qt::CTRL | Qt::Key_Space);
   playAllAction->setCheckable(false);
   connect( playAllAction, & QAction::triggered, mediaEngine_A, & IF_MediaEngineInterface::play );
   connect( playAllAction, & QAction::triggered, mediaEngine_B, & IF_MediaEngineInterface::play );
   connect( playAllAction, & QAction::triggered, lightPresetPanel, & IF_GuiLightControlPanel::triggerActivePreset);

   Action * rewindAllAction = new Action( QIcon(IconPath("track_rewind_all.png")), tr("rewind all"));
   rewindAllAction->bindShortcut( Qt::CTRL | Qt::Key_Home);
   rewindAllAction->setCheckable(false);
   connect( rewindAllAction, & QAction::triggered, mediaEngine_A, & IF_MediaEngineInterface::rewind);
   connect( rewindAllAction, & QAction::triggered, mediaEngine_B, & IF_MediaEngineInterface::rewind);

   Action * stopAllAction = new Action( QIcon(IconPath("track_stop_all.png")), tr("stop all"));
   stopAllAction->bindShortcut( Qt::Key_Escape);
   stopAllAction->setCheckable(false);
   connect( stopAllAction, & QAction::triggered, mediaEngine_A, & IF_MediaEngineInterface::stop);
   connect( stopAllAction, & QAction::triggered, mediaEngine_B, & IF_MediaEngineInterface::stop);

   m_mainWindow->addMediaListActions( QList<QAction *>() << playAllAction << pauseAllAction
                                      << rewindAllAction << stopAllAction);

   connect( mediaActionController_B, & ActionListController::activeRowChanged,
            audioVideoAutomation_B, & MediaAutomation::activateByRowNumer);

   connect( mediaActionController_B, & ActionListController::activeRowChanged,
            playlistBar_B, & PlaylistBar::onActiveRowChanged );

   /* 'show changed' notification */
   connect( lightModel, SIGNAL(layoutChanged()), m_mainWindow, SLOT(onShowChanged()) );
   connect( playlistModel_A, SIGNAL(layoutChanged()), m_mainWindow, SLOT(onShowChanged()) );
   connect( playlistModel_B, SIGNAL(layoutChanged()), m_mainWindow, SLOT(onShowChanged()) );

   /* Open Web Net function */
   OpenWebNetFactory ownFactory;
   OpenWebNetEngine_IF * ownEngine = ownFactory.buildEngine( *applicationSettings,
                                                              statusDisplay);
   OwnModel * ownModel = ownFactory.buildModel();

   OpenWebNetGUIFactory ownGUIFactory;
   OpenWebNetPanel * ownPanel =
         ownGUIFactory.buildPanel( mainWindow()->openWebNetFunctionArea(),
                                    ownModel,
                                    *ownEngine,
                                    *applicationSettings,
                                    *statusDisplay);
   connect( setEditModeAction, SIGNAL(triggered(bool)), ownPanel, SLOT(setEditMode(bool)));

   connect( mainWindow(), SIGNAL(mainWindowAboutToClose()), ownPanel, SLOT(checkToSave()));

   OwnFileLoader * ownLoader = new OwnFileLoader( *ownModel, this);

   connect( ownLoader, SIGNAL(configurationLoaded()), ownPanel, SLOT(onConfigurationFileLoaded()));

   /* load current file and listen for future changes */
   ownLoader->loadConfigurationFile( applicationSettings->getOpenWebNetSettings().configFilePath);

   connect( applicationSettings, SIGNAL(OWN_configFileChanged(QString)),
            ownLoader, SLOT(loadConfigurationFile(QString)));

   /* sequencer function */
   Sequencer::SequencerFactory sequencerFactory;
   SequenceEditorGui * sequencerGui =
         sequencerFactory.buildGuiHandler( *statusDisplay, *applicationSettings,
                                           m_mainWindow->sequencerFunctionArea());

   Sequencer::InstructionFactory * instructionFactory =
         sequencerFactory.buildInstructionFactory( *audioVideoAutomation_A, *audioVideoAutomation_B,
                                                    *lightEngine, *ownEngine, *ownModel, *sequencerGui);

   Sequencer::Functionality * sequencerFunction =
         sequencerFactory.buildFunctionality( *sequencerGui, *instructionFactory, this);

   connect( sequencerGui, & SequenceEditorGui::textChanged,
            m_mainWindow, & MainWindow::onShowChanged);

   connect( setEditModeAction, & QAction::triggered, sequencerGui, & SequenceEditorGui::setEditMode);

   QList<QAction *> sequencerActions = sequencerFactory.buildActions( sequencerFunction);
   mainWindow()->addSequencerActions( sequencerActions);
   actionMode->addActions( sequencerActions);

   /* script function */
   m_scriptFactory = new ScriptFunctionFactory( setEditModeAction, this);
   ScriptEngine *scriptEngine = m_scriptFactory->build( m_mainWindow->scriptArea(),
                                                        playlistModel_A, playlistModel_B,
                                                        sequencerFunction->entryList(),
                                                        statusDisplay, *applicationSettings,
                                                        lightModel );

   connect( setEditModeAction, SIGNAL(triggered(bool)), scriptEngine, SLOT(setEditMode(bool)) );

   wireScriptFunction( scriptEngine, audioVideoAutomation_A, audioVideoAutomation_B,
                       lightEngine, sequencerFunction);
   QList<QAction *> scriptActions = scriptEngine->getActionList();
   m_mainWindow->addScriptActions( scriptActions);
   actionMode->addActions( scriptActions);

   /* show function */
   ShowFactory *showFactory = new ShowFactory( numberOfDmxChannels, *applicationSettings, this);
   ShowManager *show = new ShowManager( showFactory, *scriptEngine, *playlistModel_A, *playlistModel_B,
                                        *lightModel, *sequencerGui, *applicationSettings, this);

   wireShowFunction( show, scriptEngine);

   /* file inport function */
   connect( fileInport, SIGNAL(inportShowFile(QString)),
            show, SLOT(loadShowFile(QString)) );
   connect( fileInport, SIGNAL(inportScriptContent(QString)),
            scriptEngine, SLOT(setScriptContent(QString)) );
   connect( fileInport, SIGNAL(inportMediaTracksLineA(QStringList) ),
            playlistModel_A, SLOT(addMediaFiles(QStringList)) );
   connect( fileInport, SIGNAL(inportMediaTracksLineB(QStringList) ),
            playlistModel_B, SLOT(addMediaFiles(QStringList)) );

   ShowFileInfo * showInfo = new ShowFileInfo( this);
   connect( show, SIGNAL(showNameChanged(QString)),
            showInfo, SLOT(onShowFilePathChanged(QString)));

   /* server function */
   Server::Factory serverFactory;

   QList<QAction *> serverActions = serverFactory.buildActions();

   mainWindow()->addServerActions( serverActions);

   Server::CommandReply_IF * cmdReply = serverFactory.buildCommandReply();

   Server::ServerEngine * serverEngine =
         serverFactory.buildEngine( serverActions.at(0), serverActions.at(1),
                                     *cmdReply, *statusDisplay);

   Server::NetworkInterface * networkProxy =
         serverFactory.buildNetworkInterface( *serverEngine, *cmdReply);

   QList<Server::Command_IF *> serverCommands;

   serverCommands = serverFactory.buildLoginCommands( *cmdReply, *applicationSettings,
                                                       *serverEngine, *networkProxy);
   serverEngine->addCommands( serverCommands);

   serverCommands = serverFactory.buildShowCommands( *cmdReply, *showInfo);
   serverEngine->addCommands( serverCommands);

   serverCommands = serverFactory.buildPlaylistCommands( *cmdReply, *audioVideoAutomation_A,
                                                          *audioVideoAutomation_B,
                                                          *playlistModel_A, *playlistModel_B,
                                                          *mediaEngine_A, *mediaEngine_B);
   serverEngine->addCommands( serverCommands);

   serverCommands = serverFactory.buildLightsetCommands( *cmdReply, *lightModel, *lightEngine);
   serverEngine->addCommands( serverCommands);

   serverEngine->addCommands( serverCommands);

   serverCommands = serverFactory.buildOwnCommands( *cmdReply, *ownModel, *ownEngine);
   serverEngine->addCommands( serverCommands);

   connect( serverActions.at(0), SIGNAL(triggered()), networkProxy, SLOT(startServer()));
   connect( serverActions.at(1), SIGNAL(triggered()), networkProxy, SLOT(stopServer()));
   connect( mainWindow(), SIGNAL(mainWindowAboutToClose()),
            networkProxy, SLOT(stopServer()) );

   connect( setEditModeAction, & QAction::triggered,
            actionMode, & ActionModeFilter::setEditMode);

   /* enter edit mode */
   setEditModeAction->setChecked( true);
   setEditModeAction->triggered( true);

}


ApplicationFactory::~ApplicationFactory()
{
   delete m_mainWindow;
}


void ApplicationFactory::wireScriptFunction( ScriptEngine *scriptEngine,
                                             MediaAutomation *avAutomationLineA,
                                             MediaAutomation *avAutomationLineB,
                                             IF_LightEngineInterface *lightEngine,
                                             Sequencer::Functionality * sequencer)
{
   connect( scriptEngine, & ScriptEngine::activateLight,
            lightEngine, & IF_LightEngineInterface::activateByName);

   connect( scriptEngine, & ScriptEngine::activateMediaLineA,
            avAutomationLineA, & MediaAutomation::activateMediaById);

   connect( scriptEngine, & ScriptEngine::activateMediaLineB,
            avAutomationLineB, & MediaAutomation::activateMediaById);

   connect( scriptEngine, & ScriptEngine::activateSequenceEntry,
            sequencer, & Sequencer::Functionality::activateEntry);

   connect( scriptEngine, & ScriptEngine::textChanged,
            m_mainWindow, & MainWindow::onShowChanged);
}

void ApplicationFactory::wireShowFunction(ShowManager *show, ScriptEngine *scriptEngine)
{
   connect( m_mainWindow, SIGNAL(loadShowFile(QString)), show, SLOT(loadShowFile(QString)) );
   connect( m_mainWindow, SIGNAL(saveShowTriggered()), show, SLOT(onSaveShowRequest()) );
   connect( m_mainWindow, SIGNAL(saveShowAsTriggered()), show, SLOT(onSaveShowAsRequest()) );
   connect( m_mainWindow, SIGNAL(openShowTriggered()), show, SLOT(openFile()) );

   connect( show, SIGNAL(scriptContent(QString)), scriptEngine, SLOT(setScriptContent(QString)) );
   connect( show, SIGNAL(showNameChanged(QString)), m_mainWindow, SLOT(onShowNameChanged(QString)) );
   connect( show, SIGNAL(showSaved(QString)), m_mainWindow, SLOT(onShowSaved(QString)) );
   connect( show, SIGNAL(newSearchPath(QString)), scriptEngine, SIGNAL(newSearchPath(QString)) );
   connect( show, SIGNAL(loadShowCorrupted()), m_mainWindow, SLOT(onShowChanged()) );

   connect( scriptEngine, SIGNAL(loadShowRequest(QString)),
            show, SLOT(loadShowFile(QString)) );
}



