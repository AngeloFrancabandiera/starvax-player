#include "FullScreenMediaWidget.h"

#include <QMainWindow>
#include <QScreen>
#include <QApplication>
#include <QVideoWidget>
#include <QLayout>
#include "testableAssert.h"

#include "StillPictureWidget.h"


FullScreenMediaWidget::FullScreenMediaWidget( QMainWindow * owner) :
   m_videoWidget( nullptr),
   m_pictureWidget( nullptr),
   m_owner( owner)
{
}

FullScreenMediaWidget::~FullScreenMediaWidget()
{
}

void FullScreenMediaWidget::attachWidgets(QVideoWidget *videoWidget, StillPictureWidget *pictureWidget)
{
   m_pictureWidget = pictureWidget;
   m_videoWidget = videoWidget;

   // _TBD these lines should not be required but, if they are removed,
   // video in line B does not appare on screen. This is under investigation.
   m_videoWidget->setParent( m_owner);
   m_pictureWidget->setParent( m_owner);
   m_videoWidget->setParent( nullptr);
   m_pictureWidget->setParent( nullptr);

   hideAll();
}

void FullScreenMediaWidget::setPixmap( const QPixmap & pixmap)
{
   T_ASSERT(m_pictureWidget);
   m_pictureWidget->setPixmap( pixmap);
}

void FullScreenMediaWidget::showVideo()
{
   smartShow( m_videoWidget);
   T_ASSERT(m_pictureWidget);
   m_pictureWidget->setVisible( false);
}

void FullScreenMediaWidget::showPicture()
{
   T_ASSERT(m_videoWidget);
   m_videoWidget->setVisible( false);
   smartShow( m_pictureWidget);
}

void FullScreenMediaWidget::hidePicture()
{
   T_ASSERT(m_pictureWidget);
   m_pictureWidget->setVisible( false);
}

bool FullScreenMediaWidget::togglePictureVisibility()
{
   T_ASSERT(m_pictureWidget);
   m_pictureWidget->isVisible() ? m_pictureWidget->hide() : smartShow( m_pictureWidget);

   return m_pictureWidget->isVisible();
}

void FullScreenMediaWidget::hideVideo()
{
   T_ASSERT(m_videoWidget);
   m_videoWidget->setVisible( false);
}

void FullScreenMediaWidget::hideAll()
{
   hideVideo();
   hidePicture();
}


void FullScreenMediaWidget::setOnTop(bool onTop)
{
   T_ASSERT(m_pictureWidget);
   bool pictVisible = m_pictureWidget->isVisible();

   if (onTop == true)
   {
      m_pictureWidget->setWindowFlags( m_pictureWidget->windowFlags() | Qt::WindowStaysOnTopHint);
   }
   else
   {
      m_pictureWidget->setWindowFlags( m_pictureWidget->windowFlags() & ( ~ Qt::WindowStaysOnTopHint));
   }

   m_pictureWidget->setVisible( pictVisible);
}


int FullScreenMediaWidget::selectScreen()
{
   int screenId = 0;  /* use default screen by default */

   if (QGuiApplication::screens().size() > 1)
   {
      /* second monitor available. Use it */
      screenId = 1;
   }

   return screenId;
}

void FullScreenMediaWidget::smartShow( QWidget * widget)
{
   int screenId = selectScreen();

   showFullScreen( widget, screenId);

   /* keep focus on main window, otherwise widgets
    * get focus and main window loses keyboard inputs */
   m_owner->activateWindow();
}


void FullScreenMediaWidget::showFullScreen( QWidget * widget, int screenId)
{
   QScreen * screen = QGuiApplication::screens().at( screenId);
   T_ASSERT( screen != nullptr);

   widget->setGeometry( screen->geometry());
   widget->showFullScreen();
}

