#ifndef LIGHTSLIDER_H
#define LIGHTSLIDER_H

#include "swiftslider.h"


/**
 * @brief The LightSlider class
 */
class LightSlider : public SwiftSlider
{
public:
   LightSlider( int maxValue, Qt::Orientation orientation,
                QWidget * parent);

   /**
    * @brief the \p channel index will be printed on the handle,
    *   if greater or equal to 0
    * @param channel
    */
   void setChannelIndex( int channel) {
      m_channelIndex = channel;
   }

   /** getter */
   int channelIndex() const {
      return m_channelIndex;
   }

protected:
   /**
    * @brief redefintion. Tooltip value is channel value in percent.
    * @param value - range [0-MAX_SLIDER_VALUE]; to be mapped to range 0% - 100%
    * @return
    */
   virtual QString formatTip(qint64 value);

   // QWidget interface
protected:
   virtual void paintEvent(QPaintEvent * event);

private:
   int m_maxValue;
   int m_channelIndex;
};

#endif // LIGHTSLIDER_H

