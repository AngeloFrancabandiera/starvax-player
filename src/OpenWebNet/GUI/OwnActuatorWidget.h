#ifndef OWNACTUATORWIDGET_H
#define OWNACTUATORWIDGET_H

#include <QWidget>
class OpenWebNetEngine_IF;
class OwnItem;
class QSlider;
class QPushButton;


/** actuator for a single light in an Open Wen Net System */
class OwnActuatorWidget : public QWidget
{
   Q_OBJECT
public:
   explicit OwnActuatorWidget( OpenWebNetEngine_IF & engine,
                               QWidget *parent = nullptr);

   /** bind an Open Web Net light. GUI is updated accordingly */
   void attachItem( const OwnItem & item);

private slots:
   void onButtonOn();
   void onButtonOff();
   void onButtonApply();

private:
   OpenWebNetEngine_IF & m_ownEngine;
   QString m_where;

   QSlider * m_slider;
   QPushButton * m_applyButton;
   QPushButton * m_onButton;
   QPushButton * m_offButton;
};

#endif // OWNACTUATORWIDGET_H
