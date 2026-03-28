#include "LightSetEditor.h"
#include <QLayout>
#include "testableAssert.h"
#include "ui_LightSetEditor.h"
#include "QDoubleSpinBox"


LightSetEditor::LightSetEditor(int numberOfChannels, QWidget *parent) :
   QGroupBox(parent),
   ui(new Ui::LightSetEditor)
{
   ui->setupUi(this);

   for (int i=0; i < numberOfChannels; i++)
   {
      QLabel * label = new QLabel( QString("%1").arg(i+1), this);
      label->setAlignment(Qt::AlignHCenter);

      QDoubleSpinBox *channelLevel = new QDoubleSpinBox(parent);
      channelLevel->setRange( 0., 100.);
      channelLevel->setDecimals( 1);

      ui->channelsLayout->addWidget( channelLevel, 0, i);
      ui->channelsLayout->addWidget( label, 1, i);

      m_levelSpinList << channelLevel;
   }
}

LightSetEditor::~LightSetEditor()
{
   delete ui;
}

QString LightSetEditor::name()
{
   return ui->name->text();
}

void LightSetEditor::setName(const QString &name)
{
   ui->name->setText( name);
}

int LightSetEditor::fadeMs()
{
   return static_cast<int>(ui->fadeTime->value() * 1000.);
}

void LightSetEditor::setFadeMs(int fadeMs)
{
   ui->fadeTime->setValue( static_cast<double>(fadeMs / 1000.));
}

QList<double> LightSetEditor::levelList()
{
   QList<double> levels;

   for (int i=0; i < m_levelSpinList.length(); i++)
   {
      levels << m_levelSpinList.at(i)->value();
   }

   T_ASSERT( levels.length() == m_levelSpinList.length());

   return levels;
}

void LightSetEditor::setLevelList(const QList<double> &levelList)
{
   T_ASSERT( levelList.length() == m_levelSpinList.length());

   for (int i=0; i < m_levelSpinList.length(); i++)
   {
      m_levelSpinList.at(i)->setValue( levelList.at(i) );
   }
}

void LightSetEditor::on_doneButton_clicked()
{
   emit editComplete( this);
}
