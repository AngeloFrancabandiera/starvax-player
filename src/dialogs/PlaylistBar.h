#ifndef PLAYLISTBAR_H
#define PLAYLISTBAR_H

#include <QStackedWidget>
class PicturePlaybar;
class AudioVideoPlayBar;
class MediaListModel;

/**
 * @brief The PlaylistBar class holds all the possible playbar that can
 *   appare in playlist view for the different kind of media.
 * This is a  stacked widget that shows the correct widget accoring to current
 * type of media.
 */
class PlaylistBar : public QStackedWidget
{
   Q_OBJECT
public:
   PlaylistBar( AudioVideoPlayBar & audioVideoPlaybar,
                PicturePlaybar & picturePlaybar,
                MediaListModel & mediaList,
                QWidget *parent = nullptr);
   ~PlaylistBar() override;

public slots:
   void onActiveRowChanged(int row);
   void onRequestToPlay();

private:
   AudioVideoPlayBar & m_audioVideoPlaybar;
   PicturePlaybar & m_picturePlaybar;
   MediaListModel & m_mediaList;
};

#endif // PLAYLISTBAR_H
