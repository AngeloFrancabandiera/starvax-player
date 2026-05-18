#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QFileDialog>
#include <QDataStream>
#include <QDockWidget>
#include <WindowLayout.h>
#include <QCloseEvent>

#include "testableAssert.h"
#include "ApplicationIcon.h"
#include "ApplicationSettings.h"
#include "AppSettingsGui.h"
#include "supported_files.h"
#include "ApplicationSettings.h"
#include "FileInport.h"
#include "LightPresetModel.h"
#include "LightEngine.h"
#include "lightControlGuiFactory.h"
#include "PlaylistDecks.h"


MainWindow::MainWindow( FileInport &fileInport,
                        ApplicationSettings & applicationSettings,
                        QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   m_fileInport(fileInport),
   m_applicationSettings(applicationSettings),
   m_appSettingsGui(nullptr)
{
   ui->setupUi(this);
   setDockNestingEnabled( true);

   setWindowIcon( ApplicationIcon("starvax.png") );
   setWindowTitle( tr("Me.te.or. Player %1.%2.%3 %4 - untitled[*]").
                   arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD).arg(VERSION_STAGE));

   retrieve_gui_elements();
   setup_gui_elems();

   setupPlaylistAreas();

   createRecentShowActions();
   connect( &m_applicationSettings, SIGNAL(recentShowsChanged()),
            this, SLOT(onRecentShowsChanged()) );
   connect_actions();

   load_action_icons();
   restoreState( m_applicationSettings.getPreviousWindowSetting() );
   restoreGeometry( m_applicationSettings.getPreviousWindowGeometry() );

   qApp->installTranslator( &m_appTranslator);
   loadApplicationLanguage();

   qApp->setApplicationVersion( QString("%1 - %2").arg(__DATE__, __TIME__));
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::retrieve_gui_elements()
{
   ui_script_anim_toolbar = findChild<QToolBar *>("script_anim_toolbar");
   ui_script_toolbar = findChild<QToolBar *>("script_toolbar");
   ui_music_toolbar = findChild<QToolBar *>("music_toolbar");
}

/** gracefully close all active services */
void MainWindow::closeEvent(QCloseEvent *ev)
{
   m_applicationSettings.saveWindowSetting( saveState());
   m_applicationSettings.saveWindowGeometry( saveGeometry());

   emit mainWindowAboutToClose();

   if (isWindowModified())
   {
      if (askToSave() != QMessageBox::Cancel)
      {
         QMainWindow::closeEvent(ev);
      }
      else
      {
         ev->ignore();
      }
   }
}

int MainWindow::askToSave()
{
   int res = QMessageBox::question( this, tr("save before exit"),
                                    tr("The show has changed. Do you want to save it?"),
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

   if (res == QMessageBox::Yes)
   {
      if (windowTitle().startsWith("Me.te.or. Player - untitled"))
      {
         emit saveShowAsTriggered();
      }
      else
      {
         emit saveShowTriggered();
      }
   }

   return res;
}

void MainWindow::openApplicationArguments()
{
   /* every argument passed when launching application
    * must be a file to be opened
    */
   for( int i = 1; i < qApp->arguments().count(); i++ )
   {
      m_fileInport.openFile( qApp->arguments().at(i) );
   }
}

void MainWindow::attachSettingsGui( AppSettingsGui * appSettingsGui)
{
   m_appSettingsGui = appSettingsGui;
}

void MainWindow::setEditMode( bool editMode)
{
   if (editMode)
   {
      loadStyleSheetSet( QStringList() << "MAIN_APP_COMM.qss" << "MAIN_APP_EDIT_MODE.qss");
   }
   else
   {
      loadStyleSheetSet( QStringList() << "MAIN_APP_COMM.qss" << "MAIN_APP_NORMAL_MODE.qss");
   }
}


void MainWindow::loadApplicationLanguage()
{
   ApplicationSettings::Language storedLanguage;
   storedLanguage = m_applicationSettings.language();
   bool isLocaleSelected = (storedLanguage == ApplicationSettings::LOCALE);

   ui->actionLocale->setChecked( isLocaleSelected);
   on_actionLocale_triggered( isLocaleSelected);
}


void MainWindow::setup_gui_elems()
{
   setAcceptDrops( false);

   // file input routing
   connect( &m_fileInport, SIGNAL(loadStyle(QString)),
            this, SLOT(loadStyleSheet(QString)) );

   QSize iconSize = QSize(m_applicationSettings.getIconSize(), m_applicationSettings.getIconSize());
   ui_script_anim_toolbar->setIconSize(iconSize);
   ui_script_toolbar->setIconSize(iconSize);
   ui_music_toolbar->setIconSize(iconSize);

   QAction *viewSequencer = ui->dockSequencerContainer->toggleViewAction();
   viewSequencer->setText(tr("Sequencer"));
   ui->menuView->insertAction( ui->menuView->actions().first(), viewSequencer);

   QAction *viewLightPreset = ui->dockLightContainer->toggleViewAction();
   viewLightPreset->setText(tr("Light Control"));
   ui->menuView->insertAction( ui->menuView->actions().first(), viewLightPreset);

   QAction *viewOpenWebNet = ui->dockOpenWebNetContainer->toggleViewAction();
   viewOpenWebNet->setText(tr("Open Web Net"));
   ui->menuView->insertAction( ui->menuView->actions().first(), viewOpenWebNet);
}


void MainWindow::setupPlaylistAreas()
{
   QAction *firstBuiltIn = ui->menuView->actions().first();

   for (int deck = 0; deck < NUMBER_OF_MEDIA_DECKS; deck++)
   {
      QString title = tr("Deck %1").arg(Playlist::toLetter(deck));

      m_dockSetForDecks[deck] = new QDockWidget(this);
      addDockWidget( Qt::RightDockWidgetArea, m_dockSetForDecks[deck]);
      m_dockSetForDecks[deck]->setWindowTitle( title);
      m_dockSetForDecks[deck]->setObjectName( title);

      /* add visibility menu for deck */
      QAction *deckVisibleAction = m_dockSetForDecks[deck]->toggleViewAction();
      deckVisibleAction->setText( title);
      ui->menuView->insertAction( firstBuiltIn, deckVisibleAction);
   }
}


void MainWindow::createRecentShowActions()
{
   ui->menuFile->addSeparator();

   for (int i = 0; i < m_applicationSettings.maxRecentShowFiles(); ++i)
   {
      QAction * recentShowAction = new QAction(this);
      recentShowAction->setVisible( false);
      m_recentShowActions.append( recentShowAction);

      connect( recentShowAction, SIGNAL(triggered()),
               this, SLOT(openRecentShow()));
   }

   /* add to menu */
   ui->menuFile->addActions( m_recentShowActions);

   /* load shows of previous sessions */
   onRecentShowsChanged();
}


void MainWindow::connect_actions()
{
   connect( ui->action_Save_Show, SIGNAL(triggered()), this, SIGNAL(saveShowTriggered()) );
   connect( ui->action_Save_Show_As, SIGNAL(triggered()), this, SIGNAL(saveShowAsTriggered()) );
   connect( ui->action_Open_Show, SIGNAL(triggered()), this, SIGNAL(openShowTriggered()) );

   connect( ui->action_Open_script, SIGNAL(triggered()),
            &m_fileInport, SLOT(openScriptDialog()) );

   ui->action_Save_Show->setEnabled( false);
   ui->action_Save_Show_As->setEnabled( true);
}

void MainWindow::load_action_icons()
{
   attachIcon( ui->action_Open_script, "open_script.png" );


   attachIcon( ui->actionOpenCurtain, "openCurtain.png" );
   attachIcon( ui->actionCloseCurtain, "closeCurtain.png" );

   attachIcon( ui->action_Open_Show, "open_show.png" );
   attachIcon( ui->action_Save_Show, "save_show.png" );
   attachIcon( ui->action_Dark_Mode, "dark_mode.png" );

   attachIcon( ui->actionAbout_MeTeOr_Player, "starvax.png");
}

void MainWindow::attachIcon( QAction *action, const QString &icon_filename)
{
   action->setIcon( ApplicationIcon( icon_filename ) );
}


QWidget *MainWindow::openWebNetFunctionArea()
{
   return ui->dockOwnPanel;
}

QWidget *MainWindow::scriptArea()
{
   return ui->scriptArea;
}

QWidget *MainWindow::sequencerFunctionArea()
{
   return ui->dockSequencer;
}

QWidget *MainWindow::connectionArea()
{
   return ui->connectionToolbar;
}

QDockWidget *MainWindow::playlistAreaForDeck(Playlist::Deck deck)
{
   return m_dockSetForDecks.at(deck);
}


QLayout *MainWindow::lightControlArea()
{
   T_ASSERT( ui->dockLightPanel->layout() != nullptr);
   return ui->dockLightPanel->layout();
}

void MainWindow::addShowActions(QList<QAction *> & actions)
{
   ui->menuMeteorShow->addActions( actions);
   ui->show_toolbar->addActions( actions);
}

void MainWindow::addMediaListActions(QList<QAction *> &actions)
{
   ui->menu_Music->addActions( actions);

   foreach (QAction *action, actions)
   {
      if (! action->icon().isNull())
      {
         ui->music_toolbar->insertAction( nullptr, action);
      }
   }

   if ( ! actions.isEmpty())
   {
      ui->music_toolbar->insertSeparator( actions.first());
   }
}

void MainWindow::addLightControlActions(QList<QAction *> actions)
{
   ui->menu_Ligth->addActions( actions);
}

void MainWindow::addSequencerActions(QList<QAction *> &actions)
{
   ui->menu_Sequence->addActions( actions);
}

void MainWindow::addScriptActions( const QList<QAction *> &actions)
{
   ui->menu_Script->addActions( actions);
   ui->script_anim_toolbar->addActions( actions);
}

void MainWindow::addServerActions(const QList<QAction *> & actions)
{
   ui->menu_Server->addActions( actions);
   ui->connectionToolbar->addActions( actions);
}


void MainWindow::onShowNameChanged(const QString &filename)
{
   setWindowTitle( QString("Me.te.or. Player %2.%3.%4 %5 - %1[*]").arg(filename)
                   .arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD).arg(VERSION_STAGE));
   ui->action_Save_Show->setEnabled( false );
   setWindowModified( false);
}

/**
 * called when any element of the show changes
 */
void MainWindow::onShowChanged()
{
   ui->action_Save_Show->setEnabled( true );
   setWindowModified( true );
}

/**
 * callback when the show is saved
 */
void MainWindow::onShowSaved( const QString & showPath)
{
   onShowNameChanged( showPath);
   ui->action_Save_Show->setEnabled( false );
   setWindowModified( false );
}


/**
 * @brief switch between native and local language
 * @param checked is TRUE for local language; FALSE for native
 */
void MainWindow::on_actionLocale_triggered(bool checked)
{
   if( checked )
   {
      // retranslate to local language
      bool res = m_appTranslator.load( "player_it",
                                       qApp->applicationDirPath() + "/res/translations" );
      T_ASSERT(res);
      m_applicationSettings.setLanguage( ApplicationSettings::LOCALE);
   }
   else
   {
      // retranslate to native language.
      bool res = m_appTranslator.load( QString(), QString() );
      (void)res;
      m_applicationSettings.setLanguage( ApplicationSettings::NATIVE);
   }

   ui->retranslateUi( this );
}

/**
 * @brief load a user stylesheet by choosing with a file browser
 */
void MainWindow::on_action_Dark_Mode_triggered()
{
   QString styleFullPath = FileInport::open_style_dialog();

   if( styleFullPath != QString() )
   {
      loadStyleSheet( styleFullPath );
   }
}

/**
 * @brief load a single stylesheet from a file path
 * @param styleFullPath is a full path
 */
void MainWindow::loadStyleSheet(const QString & styleFullPath)
{
   QFile fStyle( styleFullPath );
   bool res = fStyle.open( QIODevice::ReadOnly );

   if ((res == true) && (fStyle.isOpen()))
   {
      qApp->setStyleSheet( fStyle.readAll() );
      fStyle.close();
   }
}

void MainWindow::loadStyleSheetSet(const QStringList & fileNames)
{
   QString styleSheet;

   foreach ( QString file, fileNames)
   {
      QFile styleFile( STYLE_SUBFOLDER + file);
      bool res = styleFile.open( QIODevice::ReadOnly);

      if ((res == true) && (styleFile.isOpen()))
      {
         styleSheet += QString::fromLatin1( styleFile.readAll());
      }
      else
      {
         statusBar()->showMessage( tr("WARNING: can't open %1").arg(STYLE_SUBFOLDER + file), 2000);
      }

      styleFile.close();
   }

   qApp->setStyleSheet( styleSheet);
}


void MainWindow::onRecentShowsChanged()
{
   QStringList showFiles = m_applicationSettings.recentShowFiles();

   int numRecentFiles = qMin(showFiles.size(), m_recentShowActions.size());

   for (int i = 0; i < numRecentFiles; ++i)
   {
      QString text = tr("&%1 %2").arg(i + 1).arg( showFiles[i]);
      m_recentShowActions[i]->setText(text);
      m_recentShowActions[i]->setData( showFiles[i]);
      m_recentShowActions[i]->setVisible(true);
   }

   for (int i = numRecentFiles; i < m_recentShowActions.size(); ++i)
   {
      T_ASSERT_REP ( i < m_recentShowActions.size(), "MW: invalid index");
      m_recentShowActions[i]->setVisible(false);
   }
}

void MainWindow::openRecentShow()
{
   QAction *action = qobject_cast<QAction *>(sender());
   if (action != nullptr)
   {
      QString filePath( action->data().toString());

      if (QFileInfo::exists(filePath))
      {
         emit loadShowFile( action->data().toString() );
      }
      else
      {
         m_applicationSettings.removeRecentShowFile( filePath);
      }
   }
}

void MainWindow::on_actionOpenCurtain_triggered()
{
   QMessageBox msg(QMessageBox::NoIcon, tr("OPEN CURTAIN - Not yet implemented"),
                   tr("In order to open the curtain a motor is needed.\n"
                      "This action is not yet possible"));
   msg.setIconPixmap( QPixmap(IconPath("openCurtain.png")) );

   msg.exec();
}

void MainWindow::on_actionCloseCurtain_triggered()
{
   QMessageBox msg(QMessageBox::NoIcon, tr("CLOSE CURTAIN - Not yet implemented"),
                   tr("In order to close the curtain a motor is needed.\n"
                      "This action is not yet possible"));
   msg.setIconPixmap( QPixmap(IconPath("closeCurtain.png")) );

   msg.exec();
}


void MainWindow::on_action_Options_triggered()
{
    if (m_appSettingsGui != nullptr)
    {
       m_appSettingsGui->exec();
    }
}

void MainWindow::on_action_view_save_layout_triggered()
{
   QString path = QFileDialog::getSaveFileName( this, tr("save layout"),
                        qApp->applicationDirPath() + QDir::separator() + "res" +
                        QDir::separator() + "layout", "*.metlayout");

   if (path != QString())
   {
      QFile outFile(path);
      bool ok = outFile.open( QIODeviceBase::WriteOnly);

      if (ok)
      {
         QDataStream stream( &outFile);
         WindowLayout handler;

         handler.save( stream, saveState(), saveGeometry());
         outFile.close();
      }
   }
}

void MainWindow::on_action_view_restore_layout_triggered()
{
   QString path = QFileDialog::getOpenFileName( this, tr("save layout"),
                     qApp->applicationDirPath() + QDir::separator() + "res" +
                     QDir::separator() + "layout", "*.metlayout");

   if (path != QString())
   {
      QFile outFile(path);
      bool ok = outFile.open( QIODeviceBase::ReadOnly);

      if (ok)
      {
         QDataStream stream( &outFile);
         WindowLayout handler;
         QByteArray saved_state, saved_geometry;

         handler.load( stream, saved_state, saved_geometry);

         restoreState( saved_state);
         restoreGeometry( saved_geometry);
      }
   }
}

void MainWindow::on_actionAbout_MeTeOr_Player_triggered()
{
   QMessageBox::about( this, tr("About Starvax Me.Te.Or. Player"),
                       QString("<h2>Starvax Me.Te.Or. Player</h2>") +
                       QString("<h3><i>Build %1.%2.%3 %4</i></h3>  %5").
                       arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD).arg(VERSION_STAGE)
                       .arg(qApp->applicationVersion()) +
                       tr("<p>Custom script, music and light animation for Me.Te.Or Shows.<br/>") +
                       QString("Powered by <b><a href=\"http://qt-project.org/\">QT6</b> and "
                               "<b><a href=\"https://www.ffmpeg.org\">FFMPEG</a></b></p>"
                               "<p><pre>&nbsp;"
                               "<img src=\":/qt-project.org/qmessagebox/images/qtlogo-64.png\" width=32 height=32/>"
                               "&nbsp;&nbsp;      "
                               "<img src=\":/images/HardCodedIcons/ffmpeg_logo.png\" width=32 height=32/>"
                               "&nbsp;</pre></p>") );
}

void MainWindow::on_actionAbout_Qt_triggered()
{
   QMessageBox::aboutQt( this);
}

void MainWindow::on_actionAbout_mmedia_library_triggered()
{
   QMessageBox::about( this, tr("About Me.Te.Or. Player Multimedia library"),
                        tr("Starvax player used <a href=\"https://www.ffmpeg.org\">FFMPEG</a>"
                           " plugin as multimedia library."));
}

