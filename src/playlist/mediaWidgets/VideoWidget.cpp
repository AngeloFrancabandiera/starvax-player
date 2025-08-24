#include "VideoWidget.h"
#include <QKeyEvent>

VideoWidget::VideoWidget(QWidget *parent) :
   QVideoWidget(parent)
{
   show();
}


void VideoWidget::keyReleaseEvent(QKeyEvent *event)
{
   switch( event->key())
   {
   case Qt::Key_Space:
      emit playPauseRequest();
      break;
   case Qt::Key_PageUp:
      emit stepForwardRequest();
      break;
   case Qt::Key_PageDown:
      emit stepForwardRequest();
      break;
   case Qt::Key_Escape:
      emit quitRequest();
      break;
   case Qt::Key_H:
      emit audioOnlyRequest( true);
      break;
   default:
      break;
   }
}
