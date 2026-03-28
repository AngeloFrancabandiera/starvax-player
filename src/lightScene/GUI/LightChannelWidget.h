#ifndef LIGHTCHANNELWIDGET_H
#define LIGHTCHANNELWIDGET_H

#include <QWidget>

class QCheckBox;
class LightSlider;

class LightChannelWidget : public QWidget
{
   Q_OBJECT
public:
   explicit LightChannelWidget( int maxValue, QWidget *parent = nullptr);

   /**
    * @brief the \p channel index will be printed on the handle,
    *   if greater or equal to 0
    * @param channel
    */
   void setChannelIndex( int channel);

   /**
    * @brief set whether this channelis influenced by preset automation
    * @param manualOnly
    */
   void setManualOnly( bool manualOnly);

   /**
    * @brief set minimum and maxumim value of the slider
    * @param min
    * @param max
    */
   void setRange(int min, int max);

   /** set a given \p value to slider */
   void setValue( int value);

   /** @return the value of slider */
   int value() const;

public slots:
   /** increment level by 10% of maximum value (unless it reached 100%) */
   void stepUp();

   /** decrement level by 10% of maximum value (unless it reached 0%) */
   void stepDown();

signals:
   void valueChanged(int,int);
   /** true if it is manual only */
   void manualOnlyChanged(bool,int);

private slots:
   void onSliderMoved(int);
   void onManualOnlyClicked(bool);

private:
   QCheckBox * m_manualOnlyCheckbox;
   LightSlider * m_slider;

private:
   void makeChannelShortcut(int channel);
};

#endif // LIGHTCHANNELWIDGET_H
