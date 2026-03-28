#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qlistview.h"
#include "qsplitter.h"

#include "qgridlayout.h"
#include "qlayout.h"
#include "QLabel"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QToolBar>

#include "qtranslator.h"

class PlaylistFunction;
class LightPresetModel;
class MediaListModel;
class ApplicationSettings;
class AppSettingsGui;
class FileInport;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow( FileInport  &fileInport,
                        ApplicationSettings & applicationSettings,
                        QWidget *parent = nullptr);
   ~MainWindow() override;

   QWidget *sequencerFunctionArea();
   QWidget *openWebNetFunctionArea();
   QWidget *scriptArea();
   QWidget *connectionArea();
   QLayout *playlistAreaLineA();
   QLayout *playlistAreaLineB();
   QLayout *lightControlArea();
   QBoxLayout *volumeSliderAreaLineA();
   QBoxLayout *volumeSliderAreaLineB();

   void addShowActions( QList<QAction *> & actions);
   void addMediaListActions( QList<QAction *> & actions);
   void addLightControlActions(QList<QAction *> actions);
   void addSequencerActions( QList<QAction *> & actions);
   void addScriptActions(const QList<QAction *> &actions);
   void addServerActions( const QList<QAction *> &actions);

   void openApplicationArguments();

   void attachSettingsGui( AppSettingsGui * appSettingsGui);

signals:
   void saveShowTriggered();
   void saveShowAsTriggered();
   void openShowTriggered();
   void loadShowFile(const QString &);
   void mainWindowAboutToClose();

public slots:
   /**
    * @brief enter or exit edit mode
    * @param editMode
    */
   void setEditMode( bool editMode);

   /** to be called whenever the show has a change that must be saved in show file  */
   void onShowChanged();


protected:
   void closeEvent(QCloseEvent *) override;
   void showEvent(QShowEvent *event) override;

private:
   void setup_gui_elems();
   void setup_drop_function();
   void setupPlaylistAreas();
   void createRecentShowActions();
   void connect_actions();
   void load_action_icons();
   void attachIcon( QAction *action, const QString &icon_filename);
   void retrieve_gui_elements();
   void loadApplicationLanguage();
   int askToSave();

   /** load a set of style-sheets. They are all read and concatenated
    * before being applied.
    * @param fileNames is a list of base file names. The path is hard-coded
    *   to application folder */
   void loadStyleSheetSet(const QStringList & fileNames);

private:
   Ui::MainWindow *ui;

   FileInport  &m_fileInport;
   ApplicationSettings & m_applicationSettings;
   QTranslator m_appTranslator;

   // toolbars
   QToolBar    *ui_script_anim_toolbar;
   QToolBar    *ui_script_toolbar;
   QToolBar    *ui_music_toolbar;

   QList<QAction *> m_recentShowActions;
   AppSettingsGui * m_appSettingsGui;

   QVBoxLayout * m_volumeLayoutLineA;
   QVBoxLayout * m_volumeLayoutLineB;
   QVBoxLayout * m_playlistLayoutLineA;
   QVBoxLayout * m_playlistLayoutLineB;

private slots:
   void onShowNameChanged(const QString &filename);
   void onShowSaved(const QString & showPath);
   void loadStyleSheet(const QString & styleFullPath);
   void onRecentShowsChanged();
   void openRecentShow();

private slots:
   void on_action_Dark_Mode_triggered();
   void on_actionLocale_triggered(bool checked);
   void on_actionOpenCurtain_triggered();
   void on_actionCloseCurtain_triggered();
   void on_action_Options_triggered();
   void on_action_view_playlist_line_A_triggered(bool checked);
   void on_action_view_playlist_line_B_triggered(bool checked);
   void on_action_view_light_preset_triggered(bool checked);
   void on_action_view_open_web_net_triggered(bool checked);
   void on_action_view_sequencer_triggered(bool checked);
   void on_dockPlaylistContainer_A_visibilityChanged(bool visible);
   void on_dockLightContainer_visibilityChanged(bool visible);
   void on_dockSequencerContainer_visibilityChanged(bool visible);
   void on_dockOpenWebNetContainer_visibilityChanged(bool visible);
   void on_dockPlaylistContainer_B_visibilityChanged(bool visible);
   void on_actionAbout_MeTeOr_Player_triggered();
   void on_actionAbout_Qt_triggered();
   void on_actionAbout_mmedia_library_triggered();
};

#endif // MAINWINDOW_H
