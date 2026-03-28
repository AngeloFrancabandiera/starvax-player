#include "Fader.h"


/** time tick to update volume */
#define DEFAULT_VOLUME_UPDATE_MS    100


Fader::Fader(QObject *parent) :
   QObject(parent)
{
   m_fadeDirection = NO_FADE;
   m_volume = 0.0;
   m_endVolume = 0;

   connect( &m_volumeUpdateTimer, SIGNAL(timeout()),
            this, SLOT(onVolumeUpdateTick()) );
}

/**
 * start value of transition
 * @param startVolume - range 0..100
 */
void Fader::fadeOutFrom(int startVolume)
{
   m_volume = (qreal)startVolume;
   m_volumeUpdateTimer.start( DEFAULT_VOLUME_UPDATE_MS );
   m_fadeDirection = FADE_OUT;
}

void Fader::fadeInTo(int endVolume)
{
   m_volume = (qreal)1;  // starting from 0 does not work on some faders
   m_endVolume = endVolume;
   m_fadeDirection = FADE_IN;
   m_volumeUpdateTimer.start( DEFAULT_VOLUME_UPDATE_MS );

   /* make an immediate call to apply volume */
   updateVolumeTickIn( endVolume);
}

void Fader::stop()
{
   m_volumeUpdateTimer.stop();
   m_fadeDirection = NO_FADE;
}


void Fader::toggleFadeOutFrom(int startVolume)
{
   (m_fadeDirection == FADE_OUT) ? stop() : fadeOutFrom(startVolume);
}

void Fader::onVolumeUpdateTick()
{
   if (m_fadeDirection == FADE_OUT)
   {
      updateVolumeTickOut();
   }

   if (m_fadeDirection == FADE_IN)
   {
      updateVolumeTickIn( m_endVolume);
   }
}
