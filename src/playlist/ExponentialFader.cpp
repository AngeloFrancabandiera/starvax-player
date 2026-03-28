#include "ExponentialFader.h"


/********************************* settings ******************************/
// exponential decay factor
#define  EXPONENTIAL_FADE_FACTOR_OUT    0.95

#define  EXPONENTIAL_FADE_FACTOR_IN    0.2

// lower volume not distinguished from 0 (range is 0.0 100.0)
#define TARGET_VOLUME_ERROR          1.0
/*************************************************************************/

ExponentialFader::ExponentialFader(QObject *parent) :
   Fader(parent)
{
}


void ExponentialFader::updateVolumeTickOut()
{
   m_volume = m_volume * EXPONENTIAL_FADE_FACTOR_OUT;

   if( m_volume < TARGET_VOLUME_ERROR )
   {
      stop();
      m_volume = 0.0;
   }

   emit changeVolume( int(m_volume) );
}

void ExponentialFader::updateVolumeTickIn(int endVolume)
{
   qreal endVolmeReal = qreal(endVolume);
   /* assume volume starts lower than end volume and never goes over */
   m_volume +=  (endVolmeReal - m_volume)* EXPONENTIAL_FADE_FACTOR_IN;

   if (m_volume >= (endVolmeReal - TARGET_VOLUME_ERROR))
   {
      stop();
      m_volume = endVolume;
   }

   emit changeVolume( int(m_volume) );
}
