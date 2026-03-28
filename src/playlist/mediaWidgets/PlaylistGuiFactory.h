#ifndef PLAYLISTGUIFACTORY_H
#define PLAYLISTGUIFACTORY_H

#include <QObject>
#include "PlaylistConstants.h"

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
                                QObject *parent = nullptr);

   MediaPositionSlider *buildSeekbar(IF_MediaEngineInterface *engine, QWidget *parent);
   VolumeSlider *buildVolumeBar( IF_MediaEngineInterface *engine, QBoxLayout *container);
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
   void buildPlaylistPanel( IF_MediaEngineInterface *engine, MediaAutomation *automation,
                            MediaListModel * mediaModel,Playlist::Line line,
                            ActionListView *playlistView, FileInport *fileInport,
                            QAction * setEditModeAction, QLayout *container);
private:
   void connectOpenMediaFunction( Playlist::Line line, FileInport *fileInport, QPushButton *openMediaButton);

private:
   const ApplicationSettings & m_settings;
};

#endif // PLAYLISTGUIFACTORY_H
