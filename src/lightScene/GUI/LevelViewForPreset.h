#ifndef LEVELVIEWFORPRESET_H
#define LEVELVIEWFORPRESET_H

#include <QWidget>
#include <QList>
#include <QPen>

class LevelViewForPreset : public QWidget
{
   Q_OBJECT
public:
   explicit LevelViewForPreset(QWidget *parent = 0);

   void setLevelValues( const QList<double> &values);

   /**
    * @brief draw one channel as manual only
    * @param manualOnly
    * @param channel is zero based
    */
   void setManualOnly(bool manualOnly, int channel);

protected:
   void paintEvent(QPaintEvent *);

private:
   QList<double> m_levels;
   QList<int> m_manualChannels;
   QPainter *m_painter;
   QPen m_pen;

private:
   void writeLevelValue( const QString &textValue, const QColor &color,
                         const QRect &textArea, const QRect &clipArea);
};

#endif // LEVELVIEWFORPRESET_H
