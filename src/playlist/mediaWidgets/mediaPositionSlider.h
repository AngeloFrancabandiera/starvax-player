#ifndef MEDIA_POSITION_SLIDER_H
#define MEDIA_POSITION_SLIDER_H

#include "swiftslider.h"
#include <QDebug>

#define TWO_DIGIT_ZERO_PAD(val)    val, 2, 10, QLatin1Char('0')
#define ONE_DIGIT(val)    val, 1, 10

/**
 * @brief The MediaPositionSlider class specialises the behviour
 *        of a generic slider to be linked with the variation
 *        of media playback position, and link back media position
 *        to manual move of the slider
 */
class MediaPositionSlider : public SwiftSlider
{
   Q_OBJECT
public:
   MediaPositionSlider( QWidget *parent = nullptr) : SwiftSlider(Qt::Horizontal, parent)
   {
      m_changingTick = false;
      setOrientation( Qt::Horizontal);

      connect( this, SIGNAL(valueChanged(int)),
               this, SLOT(onValueChanged(int)) );
   }

   ~MediaPositionSlider() override {}

private:
   bool m_changingTick;    /**< set to TRUE while slider is moving for playback tick */

signals:
   void positionChangeRequested( qint64 positionMs);

protected:

   /**
    * @brief change relative position into a timestamp formatted as mm:ss .d
    *
    * @param value - slider tick value (where mouse is). Tick is assumed as 0.1 s
    *
    * @retval formatted string to be shown as tooltip
    *
    */
   virtual QString formatTip( qint64 value) override
   {
      int dec = (value % 10);
      int sec = (value / 10) % 60;
      int min = (int)((value / 10) / 60);

      return QString("<b>%1:%2</b> .%3")
            .arg(min).arg(TWO_DIGIT_ZERO_PAD(sec)).arg(ONE_DIGIT(dec));
   }

public slots:
   /** called when slider moves to follow
     * playing track */
   void setPositionMs(qint64 positionMs)
   {
      m_changingTick = true;

      /* milliseconds to tenths of second */
      int position = (int)(positionMs / 100LL);
      setValue( position );

      m_changingTick = false;
   }

   void setDurationMs(qint64 durationMs)
   {
      qDebug() << "slider total: " << (double)durationMs/1000. << " s";
      /* define one positon every 100 ms */
      int ticks = (int)( durationMs / 100LL);
      setRange( 0, ticks);
   }

   void onValueChanged(int value)
   {
      /* change local timestamp to ms timestamp */
      qint64 positionMs = (qint64)(value * 100LL);

      /* value may have changed by GUI or for playback tick */
      if (m_changingTick == false)
      {
         /* position changed by GUI */
         emit positionChangeRequested( positionMs );
      }
   }
};


#endif // MEDIA_POSITION_SLIDER_H
