#ifndef IF_MEDIAENGINEINTERFACE_H
#define IF_MEDIAENGINEINTERFACE_H

#include "QObject"
class QWidget;
class QFont;
class QColor;

namespace MediaObject
{

/* state oF audio/video objects */
typedef enum
{
   LoadingState,
   BufferingState,
   LoadedState,  /**< ready to play  */
   ErrorState
} AvMediaState;

/* state requested by user for player */
typedef enum
{
   StoppedState,
   PlayingState,
   PausedState
} AvPlayerState;

/* nature of media */
enum MediaKind {
   None = 0,
   AudioVideo,
   Picture
};

}  // namespace AudioObject

class AbstractMediaSource;


class IF_MediaEngineInterface : public QObject
{
   Q_OBJECT
public:
   IF_MediaEngineInterface( QObject *parent) : QObject(parent) {}
   virtual ~IF_MediaEngineInterface() override {}

public slots:
   /** select media to be played back */
   virtual void setCurrentSource( const AbstractMediaSource *source, bool dontStopFlag = false ) = 0;
   /** for audio/video: start playback; for images: show */
   virtual void play() = 0;
   /** for audio/video: pause playback; for images: hide */
   virtual void pause() = 0;
   /** for audio/video: toggle playback; for images: toggle hide */
   virtual void togglePlayPause() = 0;
   virtual void stop() = 0;
   virtual void rewind() = 0;
   /** move foreward of a fixed amount of time (currently 5 seconds) */
   virtual void stepForward() = 0;
   /** move backward of a fixed amount of time (currently 5 seconds) */
   virtual void stepBackward() = 0;
   /** duration of a 'step' in milliseconds */
   virtual void setStepSizeMs( int stepMs) = 0;
   /** move forward one frame of a video file. For audio only file, behavior is undefined */
   virtual void singleFrameForward() = 0;
   /** move backward one frame of a video file. For audio only file, behavior is undefined */
   virtual void singleFrameBackward() = 0;
   virtual void setVolume(int vol) = 0;
   virtual int volume() = 0;
   virtual void setTickInterval( qint32 tickMs ) = 0;
   virtual void onUserPositionRequested( qint64 positionMs) = 0;
   virtual void setMuted( bool muted) = 0;
   virtual void setAudioOnly( bool audioOnly) = 0;
   virtual void showOnTop( bool onTop) = 0;
   virtual void enableFadeIn( bool enabled) = 0;
   /** enable repetition of the same track endlessly */
   virtual void setLoopPlayback( bool enabled) = 0;

   virtual void enableSubtitles() = 0;
   virtual void disableSubtitles() = 0;

signals:
   void muteStatus( bool);
   void volumeChanged(int);
   void tick(qint64);
   void AvPlayerStateChanged( MediaObject::AvPlayerState);
   void AvMediaStateChanged( MediaObject::AvMediaState);
   void pictureShowChanged( bool );
   void finished();
   void totalTimeChanged(qint64);
   void audioOnlyChanged( bool);
   void currentMediaChanged( const AbstractMediaSource & picture);
   void onTopChanged( bool onTop);
};

#endif // IF_MEDIAENGINEINTERFACE_H
