#include "OwnActuatorWidget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>

#include "swiftslider.h"
#include "ApplicationIcon.h"
#include "OpenWebNetEngine_IF.h"
#include "OwnItem.h"
#include "testableAssert.h"



OwnActuatorWidget::OwnActuatorWidget( OpenWebNetEngine_IF & engine,
                                      QWidget *parent) :
   QWidget(parent),
   m_ownEngine(engine)
{
   setLayout( new QHBoxLayout(this));
   T_ASSERT( layout() != nullptr);

   layout()->setContentsMargins( 0,0,0,0);
   layout()->setSpacing(2);
   layout()->addItem( new QSpacerItem(10,1, QSizePolicy::MinimumExpanding));

   m_slider = new SwiftSlider(Qt::Horizontal, this);

   /* 1 is OFF; 2-10 is 20% to 100% */
   m_slider->setMinimum(1);
   m_slider->setMaximum(10);
   m_slider->setTickPosition( QSlider::TicksAbove);
   m_slider->setTickInterval( 1);
   m_slider->setPageStep(1);

   layout()->addWidget( m_slider );

   m_applyButton = new QPushButton(QIcon(IconPath("track_play")), "", this);
   m_applyButton->setMaximumWidth( 32);
   m_applyButton->setMinimumWidth( 32);
   layout()->addWidget( m_applyButton);

   m_onButton = new QPushButton(QIcon(IconPath("light")), "", this);
   m_onButton->setMaximumWidth( 60);
   m_onButton->setMinimumWidth( 50);
   layout()->addWidget( m_onButton);

   m_offButton = new QPushButton(QIcon(IconPath("light-off")), "", this);
   m_offButton->setMaximumWidth( 60);
   m_offButton->setMinimumWidth( 50);
   layout()->addWidget( m_offButton);

   connect( m_onButton, SIGNAL(released()), this, SLOT(onButtonOn()));
   connect( m_offButton, SIGNAL(released()), this, SLOT(onButtonOff()));
   connect( m_applyButton, SIGNAL(released()), this, SLOT(onButtonApply()));
}

void OwnActuatorWidget::attachItem( const OwnItem & item)
{
   m_where = item.where;
   m_slider->setVisible( item.dimmerable);
   m_applyButton->setVisible( item.dimmerable);
   m_onButton->setVisible( ! item.dimmerable);
   m_offButton->setVisible( ! item.dimmerable);
}

void OwnActuatorWidget::onButtonOn()
{
   m_ownEngine.turnOn( m_where);
}

void OwnActuatorWidget::onButtonOff()
{
   m_ownEngine.turnOff( m_where);
}

void OwnActuatorWidget::onButtonApply()
{
   m_ownEngine.dimmer( m_slider->value(), m_where);
}

