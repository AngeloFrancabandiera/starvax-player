#ifndef PLAYLISTGUIFACTORY_H
#define PLAYLISTGUIFACTORY_H

#include <QObject>
#include "PlaylistDecks.h"

class QDockWidget;
class QBoxLayout;
class QLayout;
class QWidget;
class QPushButton;
class QAction;
class QAbstractListModel;
class QVBoxLayout;

class IF_MediaEngineInterface;
class MediaListModel;
class MediaAutomation;
class MediaPositionSlider;
class VolumeSlider;
class AudioVideoPlayBar;
class PicturePlaybar;
class PlaylistBar;
class ActionListController;
class ApplicationSettings;
class ActionListView;
class StatusDisplay;
class FullScreenMediaWidget;
class FileInport;


/**
 * @brief The PlaylistGuiFactory class generates media widgets to
 * interact with a media content. Each build function takes a QWidget
 * in input that is used for parenting, (and automatic deletion), but
 * not for positioning in screen, that is demanded to caller
 */

class PlaylistGuiFactory : public QObject
{
   Q_OBJECT
public:
   explicit PlaylistGuiFactory( const ApplicationSettings & settings,
                                Playlist::Deck deck,
                                IF_MediaEngineInterface *engine,
                                MediaListModel * mediaList,
                                FileInport *fileInport,
                                QObject *parent = nullptr);

   AudioVideoPlayBar *buildAudioVideoPlaybar( QWidget *parent);

   PicturePlaybar *buildPicturePlaybar( MediaAutomation &automation, QWidget *parent);

   PlaylistBar * buildPlaylistBar( AudioVideoPlayBar * audioVideoBar,
                                   PicturePlaybar * pictureBar,
                                   QWidget * parent);

   ActionListView * buildPlaylistView( PlaylistBar * playbar,
                                       ActionListController *actionController,
                                       StatusDisplay * msgDisplay,
                                       QWidget *parent);

   void buildPlaylistPanel( MediaAutomation *automation,
                            ActionListView * playlistView,
                            QAction * setEditModeAction,
                            QDockWidget *container);
private:
   void buildPlaylistControlArea( MediaAutomation *automation,
                                  ActionListView * playlistView,
                                  QAction * setEditModeAction,
                                  QLayout *container);

   VolumeSlider *buildVolumeBar( QBoxLayout *container);
   MediaPositionSlider *buildSeekbar( QWidget *parent);

   void buildInternalLayouts( QDockWidget *container,
                              QVBoxLayout **mediaListLayout,
                              QVBoxLayout **volumeLayout);

   void connectOpenMediaFunction( QPushButton *openMediaButton);

private slots:
   void onOpenMediaClicked();

private:
   const ApplicationSettings & m_settings;
   Playlist::Deck m_deck;
   IF_MediaEngineInterface *m_engine;
   MediaListModel *m_mediaList;
   FileInport *m_fileInport;
};

#endif // PLAYLISTGUIFACTORY_H
