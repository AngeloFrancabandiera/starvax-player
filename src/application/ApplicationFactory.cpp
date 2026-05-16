#include "ApplicationFactory.h"
#include <QAction>
#include <QVideoWidget>
#include <QFontDatabase>

#include <array>

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
#include "PlaylistDecks.h"

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

namespace {
   std::array<MediaAutomation *, NUMBER_OF_MEDIA_DECKS> MediaAutomationSet;
   std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> MediaListModelSet;
   std::array<IF_MediaEngineInterface *, NUMBER_OF_MEDIA_DECKS> MediaEngineSet;
}


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

   /* 'show changed' notification */
   connect( lightModel, SIGNAL(layoutChanged()), m_mainWindow, SLOT(onShowChanged()) );

   /* playlist function */
   PlaylistFunctionFactory *playlistFactory;
   playlistFactory = new PlaylistFunctionFactory( this);

   /* action for both lines */
   Action * pauseAllAction = new Action( QIcon(IconPath("track_pause_all.png")), tr("pause all"));
   pauseAllAction->bindShortcut(Qt::Key_End);
   pauseAllAction->setCheckable(false);

   Action * playAllAction = new Action( QIcon(IconPath("track_play_all.png")), tr("play all"));
   playAllAction->bindShortcut(Qt::CTRL | Qt::Key_Space);
   playAllAction->setCheckable(false);

   Action * rewindAllAction = new Action( QIcon(IconPath("track_rewind_all.png")), tr("rewind all"));
   rewindAllAction->bindShortcut( Qt::CTRL | Qt::Key_Home);
   rewindAllAction->setCheckable(false);

   Action * stopAllAction = new Action( QIcon(IconPath("track_stop_all.png")), tr("stop all"));
   stopAllAction->bindShortcut( Qt::Key_Escape);
   stopAllAction->setCheckable(false);

   m_mainWindow->addMediaListActions( QList<QAction *>() << playAllAction << pauseAllAction
                                                         << rewindAllAction << stopAllAction);

   connect( playAllAction, & QAction::triggered, lightPresetPanel, & IF_GuiLightControlPanel::triggerActivePreset);

   // _TODO extraxt function
   for (int deck=0; deck < NUMBER_OF_MEDIA_DECKS; deck++)
   {
      FullScreenMediaWidget * mediaWidget = new FullScreenMediaWidget( m_mainWindow);

      ExponentialFader *expFader = new ExponentialFader( this);
      MediaListModel *playlistModel = playlistFactory->buildModel(QString("P%1").arg(deck+1));
      MediaListModelSet[deck] = playlistModel;

      IF_MediaEngineInterface * mediaEngine = playlistFactory->buildMediaEngine( *expFader,
                                                                                 *mediaWidget,
                                                                                 *statusDisplay,
                                                                                 applicationSettings);
      MediaEngineSet[deck] = mediaEngine;
      mediaEngine->setVolume( applicationSettings->defaultVolume(deck));  // _TODO line A?

      ActionListController * mediaActionController = new ActionListController( *playlistModel, this);

      MediaAutomation *audioVideoAutomation =
            playlistFactory->buildAutomation( playlistModel, mediaEngine,
                                              mediaActionController, expFader,
                                              applicationSettings->defaultVolume(deck),  // _TODO line A?
                                              *statusDisplay);
      MediaAutomationSet[deck] = audioVideoAutomation;

      PlaylistGuiFactory *playGuiFactory = new PlaylistGuiFactory( *applicationSettings, deck,
                                                                    mediaEngine, playlistModel,
                                                                    fileInport, this);
      AudioVideoPlayBar *audvidPlaybar = playGuiFactory->buildAudioVideoPlaybar( m_mainWindow);
      PicturePlaybar * pictPlaybar = playGuiFactory->buildPicturePlaybar( *audioVideoAutomation, m_mainWindow);
      PlaylistBar * playlistBar = playGuiFactory->buildPlaylistBar( audvidPlaybar, pictPlaybar,
                                                                    m_mainWindow);

      ActionListView *playlistView = playGuiFactory->buildPlaylistView( playlistBar,
                                                                        mediaActionController,
                                                                        statusDisplay,
                                                                        m_mainWindow);
      pictPlaybar->setThumbnailContainer( playlistView);
      playlistView->setDragEnabled(true);
      playlistView->setTitle(tr("Playlist Deck %1").arg(deck+1));

      audioVideoAutomation->activateNextAfterPlaybck( applicationSettings->activateNextAfterPLayback());
      connect( applicationSettings, & ApplicationSettings::activateNextAfterPLaybackChanged,
               audioVideoAutomation, & MediaAutomation::activateNextAfterPlaybck);


      connect( mediaActionController, & ActionListController::requestToPlay,
               mediaEngine, & IF_MediaEngineInterface::play);

      playGuiFactory->buildPlaylistPanel( audioVideoAutomation,
                                          playlistView,
                                          setEditModeAction,
                                          m_mainWindow->playlistAreaForDeck(deck));

      // _TODO setDefaultVolume is per instance and has no 'deck' parameter
//      connect( applicationSettings, & ApplicationSettings::defaultVolumeChanged,
//               audioVideoAutomation, & MediaAutomation::setDefaultVolume );

      connect( m_mainWindow, SIGNAL(mainWindowAboutToClose()), audioVideoAutomation, SLOT(onClosing()) );

      connect( mediaActionController, & ActionListController::activeRowChanged,
               audioVideoAutomation, & MediaAutomation::activateByRowNumer);

      connect( mediaActionController, & ActionListController::activeRowChanged,
               playlistBar, & PlaylistBar::onActiveRowChanged);

      /* build media Actions. Use SHIFT modifier for lINE B */
      QList<QAction *> playlistActions =
            playlistFactory->buildActionList( mediaEngine, audioVideoAutomation,
                                             PlaylistFunctionFactory::BaseActions);

      m_mainWindow->addMediaListActions( playlistActions);
      actionMode->addActions( playlistActions);

      connect( playAllAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::play );
      connect( pauseAllAction, SIGNAL(triggered()), mediaEngine, SLOT(pause()));
      connect( rewindAllAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::rewind);
      connect( stopAllAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::stop);
      connect( playlistModel, SIGNAL(layoutChanged()), m_mainWindow, SLOT(onShowChanged()) );
   }

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
         sequencerFactory.buildInstructionFactory( MediaAutomationSet, *lightEngine,
                                                   *ownEngine, *ownModel, *sequencerGui);

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
                                                        MediaListModelSet,
                                                        sequencerFunction->entryList(),
                                                        statusDisplay, *applicationSettings,
                                                        lightModel );

   connect( setEditModeAction, SIGNAL(triggered(bool)), scriptEngine, SLOT(setEditMode(bool)) );

   wireScriptFunction( scriptEngine, MediaAutomationSet,
                       lightEngine, sequencerFunction);
   QList<QAction *> scriptActions = scriptEngine->getActionList();
   m_mainWindow->addScriptActions( scriptActions);
   actionMode->addActions( scriptActions);

   /* show function */
   ShowFactory *showFactory = new ShowFactory( numberOfDmxChannels, *applicationSettings, this);
   ShowManager *show = new ShowManager( showFactory, *scriptEngine, MediaListModelSet,
                                        *lightModel, *sequencerGui, *applicationSettings, this);

   wireShowFunction( show, scriptEngine);

   /* file inport function */
   connect( fileInport, SIGNAL(importShowFile(QString)),
            show, SLOT(loadShowFile(QString)) );
   connect( fileInport, SIGNAL(importScriptContent(QString)),
            scriptEngine, SLOT(setScriptContent(QString)) );

   for (QAbstractListModel * model : MediaListModelSet)
   {
      connect( fileInport, & FileInport::importMediaTracksForDeck,
               dynamic_cast<MediaListModel*>(model), & MediaListModel::addMediaFiles );
   }

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

   serverCommands = serverFactory.buildPlaylistCommands( *cmdReply,
                                                         MediaAutomationSet,
                                                         MediaListModelSet,
                                                         MediaEngineSet);
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
                                             std::array<MediaAutomation *, NUMBER_OF_MEDIA_DECKS> & mediaAutomationSet,
                                             IF_LightEngineInterface *lightEngine,
                                             Sequencer::Functionality * sequencer)
{
   connect( scriptEngine, & ScriptEngine::activateLight,
            lightEngine, & IF_LightEngineInterface::activateByName);

   connect( scriptEngine, & ScriptEngine::activateSequenceEntry,
            sequencer, & Sequencer::Functionality::activateEntry);

   connect( scriptEngine, & ScriptEngine::textChanged,
            m_mainWindow, & MainWindow::onShowChanged);

   for (MediaAutomation * automation : mediaAutomationSet )
   {
      connect( scriptEngine, & ScriptEngine::activateMediaLineA,
               automation, & MediaAutomation::activateMediaById);
   }
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



