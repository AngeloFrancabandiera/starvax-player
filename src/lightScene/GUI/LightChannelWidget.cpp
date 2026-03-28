#include "LightChannelWidget.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <LightSlider.h>
#include <QAction>

namespace {

const QString CHECKBOX_STYLE(
"QCheckBox::indicator:unchecked {                          "
"    image: url(:/images/HardCodedIcons/automatic.png);    "
"}                                                         "
"                                                          "
"QCheckBox::indicator:checked {                            "
"    image: url(:/images/HardCodedIcons/manualOnly.png);   "
"}                                                         " );

}  // namespace

LightChannelWidget::LightChannelWidget(int maxValue, QWidget *parent) :
   QWidget(parent),
   m_manualOnlyCheckbox( new QCheckBox(this)),
   m_slider( new LightSlider( maxValue, Qt::Vertical, this))
{
   m_manualOnlyCheckbox->setText("");
   m_manualOnlyCheckbox->setStyleSheet( CHECKBOX_STYLE);
   m_manualOnlyCheckbox->setFocusPolicy( Qt::NoFocus);

   setLayout( new QVBoxLayout(this));
   layout()->setSpacing(0);
   layout()->setContentsMargins( 1,1,1,1);

   layout()->addWidget( m_manualOnlyCheckbox);
   layout()->addWidget( m_slider);

   connect( m_slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderMoved(int)) );
   connect( m_manualOnlyCheckbox, SIGNAL(clicked(bool)),
            this, SLOT(onManualOnlyClicked(bool)) );
}

void LightChannelWidget::makeChannelShortcut( int channel)
{
   if ((channel >= 1) && (channel <= 12))
   {
      QAction * increment = new QAction(tr("up 20%"), this);
      increment->setShortcut( QKeySequence(QString("CTRL+SHIFT+F%1").arg(channel)));
      this->addAction( increment);
      connect( increment, SIGNAL(triggered()), this, SLOT(stepUp()));

      QAction * decrement = new QAction(tr("down 20%"), this);
      decrement->setShortcut( QKeySequence(QString("CTRL+F%1").arg(channel)));
      this->addAction( decrement);
      connect( decrement, SIGNAL(triggered()), this, SLOT(stepDown()));

      this->setContextMenuPolicy( Qt::ActionsContextMenu);
   }
}

void LightChannelWidget::setChannelIndex(int channel)
{
   m_slider->setChannelIndex( channel);

   /* when channel is between 1 and 12, it can be controlled by function keys */
   makeChannelShortcut( channel);
}

void LightChannelWidget::setManualOnly(bool manualOnly)
{
   m_manualOnlyCheckbox->setChecked( manualOnly);
}

void LightChannelWidget::setRange(int min, int max)
{
   m_slider->setRange( min, max);
}

void LightChannelWidget::setValue(int value)
{
   m_slider->setValue( value);
}

int LightChannelWidget::value() const
{
   return m_slider->value();
}

void LightChannelWidget::stepUp()
{
   int maxValue = m_slider->maximum();
   int stepSize = 2 * maxValue / 10;
   int currentValue = m_slider->value();

   if (currentValue >= (maxValue - stepSize))
   {
      m_slider->setValue( maxValue);
   }
   else
   {
      m_slider->setValue( currentValue + stepSize);
   }
}

void LightChannelWidget::stepDown()
{
   int stepSize = 2 * m_slider->maximum() / 10;
   int currentValue = m_slider->value();

   if (currentValue <= stepSize)
   {
      m_slider->setValue( 0);
   }
   else
   {
      m_slider->setValue( currentValue - stepSize);
   }
}

void LightChannelWidget::onSliderMoved( int value)
{
   emit valueChanged( value, m_slider->channelIndex());
}

void LightChannelWidget::onManualOnlyClicked( bool isManual)
{
   emit manualOnlyChanged( isManual, m_slider->channelIndex());
}

