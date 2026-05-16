#ifndef PLAYLISTGUIFACTORY_H
#define PLAYLISTGUIFACTORY_H

#include <QObject>
#include "PlaylistDecks.h"

class QDockWidget;
class QBoxLayout;
class QLayout;
class QWidget;
class QPushButton;
class IF_MediaEngineInterface;
class MediaListModel;
class MediaAutomation;
class MediaPositionSlider;
class VolumeSlider;
class AudioVideoPlayBar;
class PicturePlaybar;
class PlaylistBar;
class QAbstractListModel;
class ActionListController;
class FileInport;
class ApplicationSettings;
class ActionListView;
class QAction;
class StatusDisplay;
class FullScreenMediaWidget;
class QVBoxLayout;


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
                                QObject *parent = nullptr);

   AudioVideoPlayBar *buildAudioVideoPlaybar( IF_MediaEngineInterface *engine, QWidget *parent);
   PicturePlaybar *buildPicturePlaybar(IF_MediaEngineInterface *engine,
                                       MediaAutomation &automation, QWidget *parent);
   PlaylistBar * buildPlaylistBar( AudioVideoPlayBar * audioVideoBar,
                                   PicturePlaybar * pictureBar, MediaListModel * mediaList,
                                   QWidget * parent);
   ActionListView * buildPlaylistView(PlaylistBar * playbar, MediaListModel *model,
                                       ActionListController *actionController,
                                       StatusDisplay * msgDisplay,
                                       QWidget *parent);
   void buildPlaylistPanel( IF_MediaEngineInterface *engine,
                            MediaAutomation *automation,
                            MediaListModel *mediaModel,
                            ActionListView * playlistView,
                            FileInport *fileInport,
                            QAction * setEditModeAction,
                            QDockWidget *container);
private:
   void buildPlaylistControlArea( IF_MediaEngineInterface *engine,
                                  MediaAutomation *automation,
                                  MediaListModel *mediaModel,
                                  ActionListView * playlistView,
                                  FileInport *fileInport,
                                  QAction * setEditModeAction,
                                  QLayout *container);

   VolumeSlider *buildVolumeBar( IF_MediaEngineInterface *engine, QBoxLayout *container);
   MediaPositionSlider *buildSeekbar(IF_MediaEngineInterface *engine, QWidget *parent);

   void buildInternalLayouts( QDockWidget *container,
                              QVBoxLayout **mediaListLayout,
                              QVBoxLayout **volumeLayout);

   void connectOpenMediaFunction( FileInport *fileInport, QPushButton *openMediaButton);

private:
   const ApplicationSettings & m_settings;
   Playlist::Deck m_deck;
   FileInport * m_fileInport;

private slots:
   void onOpenMediaClicked();
};

#endif // PLAYLISTGUIFACTORY_H
