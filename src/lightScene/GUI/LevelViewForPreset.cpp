#include "LevelViewForPreset.h"
#include <QPaintEvent>
#include <QPainter>
#include <QList>
#include <testableAssert.h>


LevelViewForPreset::LevelViewForPreset(QWidget *parent) :
   QWidget(parent),
   m_painter(nullptr)
{
}


/**
 * @brief this class must know preset values to color the bars (and write value)
 *        according to passed values.
 * @param values - list of levels in range [0-100] (percent)
 */
void LevelViewForPreset::setLevelValues(const QList<double> &values)
{
   m_levels = values;
}


void LevelViewForPreset::setManualOnly( bool manualOnly, int channel)
{
   if (manualOnly)
   {
      if (m_manualChannels.indexOf( channel) == -1)
      {
         m_manualChannels << channel;
      }
   }
   else
   {
      m_manualChannels.removeAll( channel);
   }

   update();
}

void LevelViewForPreset::paintEvent(QPaintEvent *)
{
   T_ASSERT( m_levels.length() > 0);
   int widthPerItem = width() / m_levels.length();

   m_painter = new QPainter(this);
   m_painter->setFont(QFont("MS Shell Dlg", -1, QFont::Bold));

   for (int i = 0; i < m_levels.length(); ++i)
   {
      QRect levelRect( i*widthPerItem, 2, widthPerItem * 9 / 10, height() * 9 / 10);
      int levelPercent = static_cast<int>(m_levels[i]);  /* clipped to integer value */

      bool isManual = (m_manualChannels.indexOf( i) != -1);

      T_ASSERT( levelPercent >= 0);
      T_ASSERT( levelPercent <= 100);

      int blackHeight = levelRect.height() * (100 - levelPercent) / 100;
      int yellowHeight = levelRect.height() - blackHeight;

      QRect blackRect = QRect( levelRect.topLeft(), QSize( levelRect.width(), blackHeight) );
      QRect yellowRect = QRect( blackRect.bottomLeft(), QSize( levelRect.width(), yellowHeight) );

      Qt::GlobalColor upperColor = isManual ? Qt::magenta : Qt::black;
      Qt::GlobalColor lowerColor = isManual ? Qt::magenta : Qt::yellow;
      Qt::GlobalColor upperColorText = isManual ? Qt::cyan : Qt::white;
      Qt::GlobalColor lowerColorText = isManual ? Qt::cyan : Qt::blue;

      m_painter->fillRect( blackRect, upperColor);
      m_painter->fillRect( yellowRect, lowerColor);

      /* write level twice with two colors so it's visible both in clear and dark zone */
      QString textValue =  QString("%1").arg( levelPercent );
      writeLevelValue( textValue, upperColorText, levelRect, blackRect);
      writeLevelValue( textValue, lowerColorText, levelRect, yellowRect);
   }

   delete m_painter;
}

/**
 * @brief write the string passed in \p textValue, clipped to the given \p clipArea
 * @param textValue - printed text
 * @param color - text color
 * @param textArea - Area that the text would occupy if not clipped.
 *    Needed to center the text
 * @param clipArea - clip area; used to write the text in more than one color
 */
void LevelViewForPreset::writeLevelValue(const QString &textValue, const QColor &color,
                                         const QRect &textArea, const QRect &clipArea)
{
   if (textArea.width() >= 20)
   {
      m_painter->save();

      m_pen.setColor( color );
      m_painter->setPen( m_pen );
      m_painter->setClipRect( clipArea);
      m_painter->drawText( textArea, Qt::AlignCenter, textValue );

      m_painter->restore();
   }
}
