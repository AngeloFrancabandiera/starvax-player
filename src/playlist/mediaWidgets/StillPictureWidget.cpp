#include "StillPictureWidget.h"
#include <QPainter>
#include <QPixmap>
#include <QKeyEvent>

#include "IF_MediaEngineInterface.h"

//shortcut
#define DOUBLE static_cast<double>

StillPictureWidget::StillPictureWidget(QWidget *parent) :
   QWidget(parent),
   m_pixmap(nullptr)
{

   Qt::WindowFlags flags;

   setStyleSheet("background:black;");

   flags = topLevelWidget()->windowFlags();
   topLevelWidget()->setWindowFlags( flags | Qt::SplashScreen | Qt::FramelessWindowHint );

   hide();

   // let main window keep focus, so it will handle keyboard
   topLevelWidget()->setFocusPolicy( Qt::NoFocus);
   setFocusPolicy( Qt::NoFocus);
   setAttribute( Qt::WA_ShowWithoutActivating);

   // for transparent images to show background
   setAttribute(Qt::WA_TranslucentBackground,true);
   setAutoFillBackground(false);
   setStyleSheet("background:transparent;");
}

StillPictureWidget::~StillPictureWidget()
{
}


void StillPictureWidget::setPixmap(const QPixmap & pixmap)
{
   if (m_pixmap)
      delete m_pixmap;
   m_pixmap = new QPixmap(pixmap);
   repaint();
}


void StillPictureWidget::paintEvent(QPaintEvent * )
{
   QPainter painter(this);
   painter.setBrush(Qt::black);
   painter.drawRect( this->rect());

   if (m_pixmap && (m_pixmap->height() > 0))
   {
      int pxHeight = m_pixmap->height();
      int pxWidth = m_pixmap->width();

      QRect drawRect = rect();

      double pxAspectRatio = DOUBLE(pxWidth) / DOUBLE(pxHeight);
      double screenAspectRatio = DOUBLE(drawRect.width()) / DOUBLE(drawRect.height());

      /* limit the screen rectangle used to draw picture
       * in order to keep aspect ratio and draw centered */
      if (pxAspectRatio < screenAspectRatio)
      {
         /* portrait */
         drawRect.setWidth( rect().height() * pxWidth/pxHeight);
         drawRect.translate((rect().width() - drawRect.width())/2, 0);
      }
      else
      {
         /* landscape */
         drawRect.setHeight( rect().width() * pxHeight/pxWidth);
         drawRect.translate( 0, (rect().height() - drawRect.height())/2);
      }

      /* use a composition mode that allows transparency */
      painter.setCompositionMode( QPainter::CompositionMode_Source);
      /* draw the final pixmap */
      painter.drawPixmap( drawRect,  *m_pixmap /*QPixmap()*/);
   }
}


void StillPictureWidget::keyPressEvent( QKeyEvent * event)
{
   switch (event->key())
   {
   case Qt::Key_Escape:
      emit hideRequest();
      break;

   case Qt::Key_Space:
      // if we get here, picture is being shown.
      // Once we hide it, we can't toggle it to be shown.
      emit hideRequest();
      break;

   default:
      break;
   }
}
