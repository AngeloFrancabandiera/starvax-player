#include "MediaAutomation.h"

#include "IF_MediaEngineInterface.h"
#include "AudioVideoSource.h"
#include "MediaListModel.h"
#include "ActionListController.h"
#include "Fader.h"
#include "testableAssert.h"
#include "StatusDisplay.h"

MediaAutomation::MediaAutomation( IF_MediaEngineInterface *engine,
                                  MediaListModel *model,
                                  ActionListController *controller, Fader *fader,
                                  int defaultVolume,
                                  StatusDisplay & msgDisplay,
                                  QObject *parent) :
   QObject( parent),
   m_engine( engine),
   m_model( model),
   m_controller( controller),
   m_fader( fader),
   m_msgDisplay( msgDisplay),
   m_currentSource( nullptr),
   m_activateNextAfterPlayback( true),
   m_defaultVolume( defaultVolume)
{
   connect( m_engine, & IF_MediaEngineInterface::finished,
            this, & MediaAutomation::onTrackFinished );
   connect( m_fader, & Fader::changeVolume, m_engine, & IF_MediaEngineInterface::setVolume );
}

/**
 * Active current index may change for different reasons: a new track
 * selected, tracks above deleted without changing the active one, or
 * active track deleted itself.
 */
void MediaAutomation::activateByRowNumer( int activeRow, bool dontStopFlag)
{
   /* check if a track has been activated */
   if (activeRow >= 0)
   {
      /* valid track activated */
      QModelIndex index = m_model->index(activeRow);
      const AbstractMediaSource *source = m_model->mediaData( index);

      applyMediaSource( source, dontStopFlag);
   }
   else
   {
      /* no active entry: it might have been erased from playlist
       * or de-activated */
      m_engine->stop();
      m_engine->setCurrentSource( nullptr);
      m_currentSource = nullptr;
   }
}


void MediaAutomation::applyMediaSource(const AbstractMediaSource * mediaSource, bool dontStopFlag)
{
   T_ASSERT( mediaSource != nullptr);

   /* if the new requested media source is the same that is already active, do nothing.
    * This prevents from stopping media in drag n drop operations */
   bool isSameMedia = ((m_currentSource != nullptr) &&
                       (mediaSource->stringId() == m_currentSource->stringId()) );

   if ( ! isSameMedia)
   {
      /* track effectively changed. */
      m_currentSource = mediaSource;

      m_engine->setCurrentSource( m_currentSource, dontStopFlag );
   }
}

void MediaAutomation::activateMediaById( const QString & stringId,
                                         bool startImmediately, bool dontStopFlag)
{
   QModelIndex target = m_model->searchById( stringId);

   if (target != QModelIndex())
   {

      m_controller->activateRequest( target.row(), startImmediately, dontStopFlag);
   }
   else
   {
      m_msgDisplay.showMessage( tr("No media has name: <b>%1</b>").arg(stringId),
                                StatusDisplay::WARNING);
   }
}

void MediaAutomation::activateNext()
{
   int activeRow = m_controller->activeItemRow();

   if ((activeRow != ActionListController::NoItemActive) &&
       (activeRow < (m_model->rowCount() - 1)))
   {
      /* activate immediately without stopping previous track.
       * This is useful in slide-show-like picture handling */
      m_controller->activateRequest( activeRow + 1, false, true);
   }
}

void MediaAutomation::activatePrevious()
{
   int activeRow = m_controller->activeItemRow();

   if ((activeRow != ActionListController::NoItemActive) &&
       (activeRow > 0) )
   {
      /* activate immediately without stopping previous track.
       * This is useful in slide-show-like picture handling */
      m_controller->activateRequest( activeRow - 1, false, true);
   }
}

/**
 * start or stop volume fading.
 */
void MediaAutomation::toggleFader()
{
   m_fader->toggleFadeOutFrom( m_engine->volume());
}

/**
 * actions taken when media functionality is about to be closed
 */
void MediaAutomation::onClosing()
{
   m_engine->stop();
}


/**
 * set audio volume to default value. If a fade is in progress,
 * it is blocked.
 */
void MediaAutomation::applyDefaultVolume()
{
   m_fader->stop();
   m_engine->setVolume( m_defaultVolume);
}

void MediaAutomation::play()
{
   m_engine->play();
}

void MediaAutomation::pause()
{
   m_engine->pause();
}

void MediaAutomation::stop()
{
   m_engine->stop();
}

void MediaAutomation::rewind()
{
   m_engine->rewind();
}

void MediaAutomation::fadeout()
{
   m_fader->fadeOutFrom( m_engine->volume());
}

void MediaAutomation::setOnTop(bool onTop)
{
   m_engine->showOnTop( onTop);
}

void MediaAutomation::setVolume(int volume)
{
   m_fader->stop();
   m_engine->setVolume( volume);
}

/**
 * When one track is over, next one is activated without playing
 */
void MediaAutomation::onTrackFinished()
{
   int activeRow = m_controller->activeItemRow();

   if (activeRow != ActionListController::NoItemActive)
   {
      if (m_activateNextAfterPlayback &&
          (activeRow < (m_model->rowCount() - 1)) )
      {
         m_controller->activateRequest( activeRow + 1);
      }

      if (m_activateNextAfterPlayback == false)
      {
         m_engine->rewind();
      }
   }

   emit trackFinished();
}

