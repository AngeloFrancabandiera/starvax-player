#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>

class VideoWidget : public QVideoWidget
{
   Q_OBJECT
public:
   VideoWidget( QWidget * parent = nullptr);

signals:
   void playPauseRequest();
   void stepForwardRequest();
   void stepBackwardRequest();
   void quitRequest();
   void audioOnlyRequest( bool audioOnly);

   // QWidget interface
protected:
   void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // VIDEOWIDGET_H
