#include "LightSlider.h"
#include "testableAssert.h"
#include <QPainter>
#include <QPaintEvent>

LightSlider::LightSlider( int maxValue,
                          Qt::Orientation orientation,
                          QWidget * parent)
   : SwiftSlider( orientation, parent),
     m_maxValue( maxValue),
     m_channelIndex(-1)
{
}

QString LightSlider::formatTip(qint64 value)
{
   T_ASSERT( m_maxValue != 0);
   return QString("%1 %").arg( static_cast<double>(value)*100. / static_cast<double>(m_maxValue), 0, 'f', 1 );
}


void LightSlider::paintEvent(QPaintEvent * event)
{
   /* add to normal paint the channel of this slide */
   SwiftSlider::paintEvent( event);

   if (m_channelIndex >= 0)
   {
      QPainter painter(this);
      int fontSize = (m_channelIndex >= 10) ? 12 : 15;

      painter.setFont( QFont("helvetica", fontSize, QFont::Bold));
      painter.setPen(QColor(220,170,0));
      painter.drawText( event->rect(), Qt::AlignCenter,
                        QString("%1").arg(m_channelIndex));
   }
}

