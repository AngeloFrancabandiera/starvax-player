#ifndef MEDIA_AUTOMATION_H
#define MEDIA_AUTOMATION_H

#include <QObject>
#include <PlaylistConstants.h>

class IF_MediaEngineInterface;
class MediaListModel;
class QModelIndex;
class AbstractMediaSource;
class ActionListController;
class Fader;
class StatusDisplay;


class MediaAutomation : public QObject
{
   Q_OBJECT
public:
   explicit MediaAutomation( IF_MediaEngineInterface *engine, MediaListModel *model,
                             ActionListController *controller, Fader *fader,
                             int defaultVolume, StatusDisplay & msgDisplay,
                             QObject *parent = nullptr);

public slots:
   /** set the playbar to a given row (i.e. \p activeRow).
    * Does not start playback.
    * If \p dontStopFlag is True, current track is not stopped (or hidden)
    */
   void activateByRowNumer(int activeRow, bool dontStopFlag = false);
   void activateMediaById(const QString & stringId, bool startImmediately=false, bool dontStopFlag=false);
   void activateNext();
   void activatePrevious();

   void toggleFader();
   void onClosing();

   void applyDefaultVolume();

   /**
    * @param activateNext: when true, after playback finishes spontaneoulsy,
    *           the next track is activated. When false, the same track is rewinded.
    */
   void activateNextAfterPlaybck( bool activateNext) {
      m_activateNextAfterPlayback = activateNext;
   }

public:
   int defaultVolume() const {
      return m_defaultVolume;
   }

   void play();
   void pause();
   void stop();
   void rewind();
   void fadeout();
   void setOnTop( bool onTop);

   void setVolume( int volume);

signals:
   /** emitted when a track has completed playback.
    * Not emitted for still images or when user stops track.
    */
   void trackFinished();

private slots:
   void setDefaultVolume( int volume) {
      m_defaultVolume = volume;
   }

   void onTrackFinished();

private:
   void applyMediaSource(const AbstractMediaSource *audioSource, bool dontStopFlag=false);

private:
   IF_MediaEngineInterface *m_engine;
   MediaListModel *m_model;
   ActionListController *m_controller;
   Fader *m_fader;
   StatusDisplay & m_msgDisplay;
   const AbstractMediaSource *m_currentSource;
   bool m_activateNextAfterPlayback;

   int m_defaultVolume;  /** a preferred volume level to be set when no fading occurs */
};

#endif // MEDIA_AUTOMATION_H

