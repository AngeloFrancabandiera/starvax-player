#ifndef MEDIAENGINE_IMP_H
#define MEDIAENGINE_IMP_H

#include "IF_MediaEngineInterface.h"
#include "AbstractMediaSource.h"

#include <QTimer>
#include <QMediaPlayer>


class Fader;
class FullScreenMediaWidget_IF;
class ApplicationSettings;
class StatusDisplay;
class QPixmap;
class VideoWidget;
class StillPictureWidget;


/** this class will aggregate specific objects for plugin used
 *  in implementation. As it is not easily testable, implementation
 *  should be simple and with as few logic as possible.
 */
class MediaEngineImp : public IF_MediaEngineInterface
{
   Q_OBJECT
public:
   explicit MediaEngineImp(Fader & fader,
                           FullScreenMediaWidget_IF & displayWidget,
                           StatusDisplay & logger,
                           QObject *parent = nullptr);
   ~MediaEngineImp() override;

public slots:
   void setGlobalEnable( bool enabled) override {
      m_globalEnable = enabled;
   }
   void setCurrentSource( const AbstractMediaSource *source, bool dontStopFlag = false ) override;
   void play() override;
   void pause() override;
   void togglePlayPause() override;
   void stop() override;
   void rewind() override;
   void setStepSizeMs( int stepMs) override;
   void stepForward() override;
   void stepBackward() override;
   void singleFrameForward() override;
   void singleFrameBackward() override;
   void setVolume(int vol) override;
   int volume() override;
   void setTickInterval(qint32 tick_ms ) override;
   void onUserPositionRequested( qint64 positionMs) override;
   void setMuted(bool isMuted) override;
   void setAudioOnly(bool audioOnly) override;
   void showOnTop( bool onTop) override;
   void enableFadeIn( bool enabled) override;
   void setLoopPlayback(bool) override;
   void enableSubtitles() override;
   void disableSubtitles() override;


private slots:
   void onAudioOnlyRequest();
   void onErrorOccurred( QMediaPlayer::Error error, const QString &errorString);
   void onPlaybackStateChanged( QMediaPlayer::PlaybackState newState);
   void onMediaStatusChanged( QMediaPlayer::MediaStatus status);
   void onDurationChanged( qint64 duration);
   void onPositionChanged( qint64 position);
   void onHasVideoChanged( bool videoAvailable);


private:
   void evaluateDisplayShow();
   void deletePixmap();

private:
   bool m_globalEnable;
   Fader & m_fader;
   StatusDisplay & m_logger;
   FullScreenMediaWidget_IF & m_displayWidget;
   bool m_videoTrackAvailable;  /* current media has a video track */
   bool m_audioOnlyRequest;  /* user has requested to hide video */
   qint32 m_tickMs;
   int m_stepSizeMs;
   bool m_fadeInFlag;
   bool m_imageFileFlag;
   QPixmap * m_pixmap;
   QString m_currentMediaPath;
   QMediaPlayer m_player;
   MediaObject::AvPlayerState m_requestedState;
   StillPictureWidget * m_pictureWidget;
   VideoWidget * m_videoWidget;
};


#endif // MEDIAENGINE_IMP_H
