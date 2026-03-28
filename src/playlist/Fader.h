#ifndef FADER_H
#define FADER_H

#include <QObject>
#include "qtimer.h"

class Fader : public QObject
{
   Q_OBJECT
public:
   explicit Fader(QObject *parent = nullptr);

   /**
    * @brief start to lower volume from \a startVolume to 0
    * @param startVolume
    */
   void fadeOutFrom( int startVolume);

   /**
    * @brief set volume to 0 and then start increasing volume
    *        up to \a endVolume
    * @param endVolumeVolume
    */
   void fadeInTo( int endVolume);

   /** stop changing volume */
   void stop();

   void toggleFadeOutFrom( int startVolume);

protected:
   enum Direction
   {
      NO_FADE = 0,
      FADE_IN = 1,
      FADE_OUT = 2
   };

private:
   QTimer m_volumeUpdateTimer;
   enum Direction m_fadeDirection;
   int m_endVolume;

protected:
   qreal m_volume;

signals:
   void changeVolume( int );

private slots:
   void onVolumeUpdateTick();

protected :
   virtual void updateVolumeTickOut() = 0;
   virtual void updateVolumeTickIn( int endVolume) = 0;

};

#endif // CFADER_H
