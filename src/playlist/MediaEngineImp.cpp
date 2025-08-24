#include "MediaEngineImp.h"

#include <QFileInfo>
#include <QPixmap>
#include <QMap>
#include <QAudioOutput>
#include <QLayout>

#include <QWindow>

#include "VideoWidget.h"
#include "StillPictureWidget.h"
#include "Fader.h"
#include "testableAssert.h"
#include "StatusDisplay.h"
#include "FullScreenMediaWidget_IF.h"
#include "supported_files.h"


namespace {
NullMediaSource NullObject;


const qint32 DEFAULT_TICK_ms = 100;
const int DEFAULT_STEP_ms = 1000;

// TBD: can we detect the file format not using file extention?
bool checkForImageFormat( const QString & extention)
{
   return PICTURE_SUPPORTED_FORMATS.contains(extention.toLower());
}

}

/**
 * setup all media objects connections and make internal connections
 */
MediaEngineImp::MediaEngineImp( Fader & fader,
                                FullScreenMediaWidget_IF &displayWidget,
                                StatusDisplay & logger,
                                QObject *parent) :
   IF_MediaEngineInterface( parent),
   m_fader( fader),
   m_logger( logger),
   m_displayWidget( displayWidget),
   m_videoTrackAvailable(false),
   m_audioOnlyRequest( false),
   m_tickMs( DEFAULT_TICK_ms),
   m_stepSizeMs( DEFAULT_STEP_ms),
   m_fadeInFlag( false),
   m_imageFileFlag( false),
   m_pixmap( nullptr),
   m_requestedState( MediaObject::StoppedState),
   m_pictureWidget( new StillPictureWidget()),
   m_videoWidget( new VideoWidget())
{
   qRegisterMetaType<MediaObject::AvPlayerState>("MediaObject::AvPlayerState");

   QAudioOutput * audioOut = new QAudioOutput( this);
   m_player.setAudioOutput( audioOut);

   m_player.setVideoOutput( m_videoWidget);
   m_videoWidget->setFullScreen( true);

   connect( & m_player, & QMediaPlayer::errorOccurred, this, & MediaEngineImp::onErrorOccurred );
   connect( & m_player, & QMediaPlayer::playbackStateChanged, this, & MediaEngineImp::onPlaybackStateChanged );
   connect( & m_player, & QMediaPlayer::mediaStatusChanged, this, & MediaEngineImp::onMediaStatusChanged );
   connect( & m_player, & QMediaPlayer::durationChanged, this, & MediaEngineImp::onDurationChanged );
   connect( & m_player, & QMediaPlayer::positionChanged, this, & MediaEngineImp::onPositionChanged );
   connect( & m_player, & QMediaPlayer::hasVideoChanged, this, & MediaEngineImp::onHasVideoChanged );

   connect( m_pictureWidget, & StillPictureWidget::hideRequest, this, & IF_MediaEngineInterface::stop);

   connect( m_videoWidget, & VideoWidget::playPauseRequest, this, & IF_MediaEngineInterface::togglePlayPause);
   connect( m_videoWidget, & VideoWidget::stepForwardRequest, this, & IF_MediaEngineInterface::togglePlayPause);
   connect( m_videoWidget, & VideoWidget::stepBackwardRequest, this, & IF_MediaEngineInterface::togglePlayPause);
   connect( m_videoWidget, & VideoWidget::audioOnlyRequest, this, & IF_MediaEngineInterface::setAudioOnly);
   connect( m_videoWidget, & VideoWidget::quitRequest, this, & IF_MediaEngineInterface::stop);

   m_displayWidget.attachWidgets( m_videoWidget, m_pictureWidget);
}


MediaEngineImp::~MediaEngineImp()
{
   delete m_videoWidget;
   delete m_pictureWidget;
}


void MediaEngineImp::deletePixmap()
{
   m_displayWidget.setPixmap( QPixmap());

   if (m_pixmap)
   {
      delete m_pixmap;
      m_pixmap = nullptr;
   }
}

void MediaEngineImp::setCurrentSource( const AbstractMediaSource *source, bool dontStopFlag)
{
   if (source != nullptr)
   {
      const QString & filePath = source->fileName();

      if (filePath != m_currentMediaPath)
      {
         if (dontStopFlag == false)  /* default case */
         {
            stop();
         }

         m_imageFileFlag = checkForImageFormat( QFileInfo(filePath).suffix());
         deletePixmap();

         if (m_imageFileFlag)
         {
            m_pixmap = new QPixmap( filePath);
            m_displayWidget.setPixmap( *m_pixmap);
         }
         else
         {
            m_player.setSource( QUrl::fromLocalFile(filePath));
            emit tick(0);

         }

         m_currentMediaPath = filePath;
         emit currentMediaChanged( *source);
      }
   }
}


void MediaEngineImp::play()
{
   m_requestedState = MediaObject::PlayingState;

   if (m_imageFileFlag)
   {
      emit pictureShowChanged(true);
   }
   else
   {
      if (m_fadeInFlag)
      {
         m_fader.fadeInTo( volume());
         m_fadeInFlag = false;
      }

      m_player.play();
   }

   evaluateDisplayShow();
}

void MediaEngineImp::pause()
{
   m_requestedState = MediaObject::PausedState;

   if (m_imageFileFlag)
   {
      emit pictureShowChanged(false);
   }
   else
   {
      m_player.pause();
   }

   evaluateDisplayShow();
}

void MediaEngineImp::togglePlayPause()
{
   if (m_imageFileFlag)
   {
      bool nowVisible = m_displayWidget.togglePictureVisibility();
      emit pictureShowChanged(nowVisible);
   }
   else
   {
      if (m_player.playbackState() == QMediaPlayer::PlayingState)
      {
         pause();
      }
      else
      {
         play();
      }
   }
}


void MediaEngineImp::stop()
{
   m_requestedState = MediaObject::StoppedState;
   m_player.stop();

   evaluateDisplayShow();

   /* used by UI controls, not by screen */
   emit pictureShowChanged(false);
   emit tick(0);
}


void MediaEngineImp::evaluateDisplayShow()
{
   if (m_imageFileFlag)
   {
      m_displayWidget.hideVideo();

      if ((m_requestedState == MediaObject::PlayingState) &&
          (m_pixmap != nullptr) )
      {
         m_displayWidget.showPicture();
      }
      else
      {
         m_displayWidget.hidePicture();
      }
   }
   else // audio-video
   {
      m_displayWidget.hidePicture();

      if ((m_audioOnlyRequest == true) ||
          (m_videoTrackAvailable == false) ||
          (m_requestedState == MediaObject::StoppedState) )
      {
         m_displayWidget.hideVideo();
      }
      else
      {
         m_displayWidget.showVideo();
      }
   }
}


/** bring track back at beginning */
void MediaEngineImp::rewind()
{
   m_player.setPosition(0);
   emit tick(0);
}

void MediaEngineImp::setStepSizeMs(int stepMs)
{
   m_stepSizeMs = stepMs;
}

void MediaEngineImp::stepForward()
{
   m_player.setPosition( m_player.position() + m_stepSizeMs);
   emit tick( m_player.position());
}

void MediaEngineImp::stepBackward()
{
   m_player.setPosition( m_player.position() - m_stepSizeMs);
   emit tick( m_player.position());
}


void MediaEngineImp::singleFrameForward()
{
   /* not supported */
}

void MediaEngineImp::singleFrameBackward()
{
   /* not supported */
}

void MediaEngineImp::onUserPositionRequested( qint64 positionMs)
{
   m_player.setPosition( positionMs);

   /* fix GUI in case position is sought before playback */
   emit tick( positionMs);
}

void MediaEngineImp::setMuted(bool isMuted)
{
   m_player.audioOutput()->setMuted( isMuted);
   emit muteStatus( isMuted);
}

void MediaEngineImp::setAudioOnly( bool audioOnly)
{
   m_audioOnlyRequest = audioOnly;
   emit audioOnlyChanged( audioOnly);

   evaluateDisplayShow();
}

void MediaEngineImp::showOnTop(bool onTop)
{
   m_displayWidget.setOnTop( onTop);
   emit onTopChanged( onTop);
}


void MediaEngineImp::enableFadeIn(bool enabled)
{
   m_fadeInFlag = enabled;
}

void MediaEngineImp::setLoopPlayback(bool enabled)
{
   m_player.setLoops( enabled ? QMediaPlayer::Infinite : QMediaPlayer::Once);
}

void MediaEngineImp::setVolume(int vol)
{
   m_player.audioOutput()->setVolume( (float)vol/100.f);

   // valid also if 'vol' is zero
   emit muteStatus( false);

   // update GUIs
   emit volumeChanged( vol);
}

int MediaEngineImp::volume()
{
   return (int)(m_player.audioOutput()->volume() * 100.f);
}


void MediaEngineImp::setTickInterval( qint32 tick_ms )
{
   m_tickMs = tick_ms;
}


void MediaEngineImp::enableSubtitles()
{
   m_player.setActiveSubtitleTrack( 1);
}


void MediaEngineImp::disableSubtitles()
{
   m_player.setActiveSubtitleTrack( -1);
}


void MediaEngineImp::onAudioOnlyRequest()
{
   emit audioOnlyChanged( true);
}

void MediaEngineImp::onErrorOccurred(QMediaPlayer::Error /*error*/, const QString &errorString)
{
   m_logger.showMessage(tr("Media Error: %1").arg(errorString), StatusDisplay::WARNING);
   emit AvMediaStateChanged( MediaObject::ErrorState);
}

void MediaEngineImp::onPlaybackStateChanged( QMediaPlayer::PlaybackState newState)
{
   static const QMap< QMediaPlayer::PlaybackState, MediaObject::AvPlayerState> PLAYER_STATE_TABLE =
   {
      { QMediaPlayer::StoppedState, MediaObject::StoppedState},
      { QMediaPlayer::PlayingState, MediaObject::PlayingState},
      { QMediaPlayer::PausedState, MediaObject::PausedState}
   };

   MediaObject::AvPlayerState avState = PLAYER_STATE_TABLE.value( newState, MediaObject::StoppedState);

   emit AvPlayerStateChanged( avState);
}

void MediaEngineImp::onMediaStatusChanged( QMediaPlayer::MediaStatus status)
{
   switch (status)
   {
   default:
   case QMediaPlayer::NoMedia:
      break;
   case QMediaPlayer::LoadingMedia:
      emit AvMediaStateChanged( MediaObject::LoadingState);
      break;
   case QMediaPlayer::LoadedMedia:
      emit AvMediaStateChanged( MediaObject::LoadedState);
      break;
   case QMediaPlayer::StalledMedia:
      emit AvMediaStateChanged( MediaObject::ErrorState);
      break;
   case QMediaPlayer::BufferingMedia:
      emit AvMediaStateChanged( MediaObject::BufferingState);
      break;
   case QMediaPlayer::BufferedMedia:
      emit AvMediaStateChanged( MediaObject::LoadedState);
      break;
   case QMediaPlayer::EndOfMedia:
      emit finished();
      break;
   case QMediaPlayer::InvalidMedia:
      emit AvMediaStateChanged( MediaObject::ErrorState);
      break;
   }
}

void MediaEngineImp::onDurationChanged( qint64 duration)
{
   emit totalTimeChanged( duration);
}

void MediaEngineImp::onPositionChanged( qint64 position)
{
   emit tick( position);
}

void MediaEngineImp::onHasVideoChanged( bool videoAvailable)
{
   m_videoTrackAvailable = videoAvailable;
   evaluateDisplayShow();
}

