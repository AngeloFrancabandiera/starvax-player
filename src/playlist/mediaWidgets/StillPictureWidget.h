#ifndef STILLPICTUREWIDGET_H
#define STILLPICTUREWIDGET_H

#include <QWidget>

class StillPictureWidget : public QWidget
{
   Q_OBJECT
public:
   explicit StillPictureWidget( QWidget *parent = nullptr);
   ~StillPictureWidget() override;

   void setPixmap( const QPixmap & pixmap);

signals:
   void hideRequest();

   // QWidget interface
protected:
   void paintEvent(QPaintEvent * event) override;
   void keyPressEvent(QKeyEvent * event) override;

private:
   const QPixmap * m_pixmap;
};

#endif // STILLPICTUREWIDGET_H
