#include "PicturePlaybar.h"
#include "ui_PicturePlaybar.h"
#include <QAbstractScrollArea>
#include "AbstractMediaSource.h"
#include "MediaAutomation.h"
#include <QCommonStyle>

#include "ApplicationIcon.h"

namespace  {
const int THUMBNAIL_SIZE = 64u;
}  // namespace

#define SHOW_BUTTON_ICON_PATH   IconPath("track_play.png")
#define PICTURE_BUTTON_ICON_PATH  IconPath("picture.png")

#define IMAGE_HIDDEN_STYLE  "QWidget{ background-color: red }; QWidget::!enabled{ background-color: gray };"
#define IMAGE_SHOWN_STYLE   "QWidget{ background-color: green }; QWidget::!enabled{ background-color: gray };"


PicturePlaybar::PicturePlaybar( IF_MediaEngineInterface * engine,
                                MediaAutomation & automation,
                                QWidget *parent) :
   QWidget(parent),
   ui(new Ui::PicturePlaybar),
   m_engine( engine),
   m_thumbnailContainer( nullptr)
{
   ui->setupUi(this);

   QCommonStyle style;
   ui->nextButton->setIcon(style.standardIcon(QStyle::SP_MediaSeekForward));
   ui->previousButton->setIcon(style.standardIcon(QStyle::SP_MediaSeekBackward));

   ui->showButton->setEnabled(true);
   ui->hideButton->setEnabled(true);
   ui->nextButton->setEnabled(true);
   ui->previousButton->setEnabled( true);

   connect( ui->showButton, & QPushButton::clicked, m_engine, & IF_MediaEngineInterface::play );
   connect( ui->hideButton, & QPushButton::clicked, m_engine, & IF_MediaEngineInterface::pause );
   connect( ui->nextButton, & QPushButton::clicked, & automation, & MediaAutomation::activateNext );
   connect( ui->previousButton, & QPushButton::clicked, & automation, & MediaAutomation::activatePrevious );

   connect( m_engine, & IF_MediaEngineInterface::currentMediaChanged,
            this, & PicturePlaybar::onCurrentMediaChanged );

   connect( m_engine, & IF_MediaEngineInterface::pictureShowChanged,
            this, & PicturePlaybar::onPictureShowChanged);

   ui->thumbnail->setVisible( false);

   ui->showButton->setIcon( QIcon(PICTURE_BUTTON_ICON_PATH));
   ui->hideButton->setIcon( QIcon(PICTURE_BUTTON_ICON_PATH));

   ui->hideButton->setStyleSheet( IMAGE_HIDDEN_STYLE);
   ui->showButton->setStyleSheet( IMAGE_SHOWN_STYLE);

   ui->showButton->setEnabled( true);
   ui->hideButton->setEnabled( false);
}

PicturePlaybar::~PicturePlaybar()
{
   delete ui;
}

/**
 * \p container is the playlist View that holds the action-bar.
 * Thumbnail area is parented to the container, so the widget must not
 * be limited to action-bar.
 */
void PicturePlaybar::setThumbnailContainer(QAbstractScrollArea * container)
{
   m_thumbnailContainer = container;

   if (m_thumbnailContainer)
   {
      ui->thumbnail->setParent( container);
   }
}

void PicturePlaybar::showPicture()
{
   m_engine->play();
}


void PicturePlaybar::onPictureShowChanged( bool isShown)
{
   if (isShown)
   {
      ui->showButton->setEnabled( false);
      ui->hideButton->setEnabled( true);
   }
   else
   {

      ui->showButton->setEnabled( true);
      ui->hideButton->setEnabled( false);
   }
}


void PicturePlaybar::onCurrentMediaChanged(const AbstractMediaSource & mediaSource)
{
   if ((mediaSource.mediaKind() == AbstractMediaSource::PICTURE_LINE_A) ||
       (mediaSource.mediaKind() == AbstractMediaSource::PICTURE_LINE_B) )
   {
      QPixmap pixmap = mediaSource.mediaContent().value<QPixmap>();

      /* if new media is not a picture, hide viewer */
      if (pixmap.isNull())
      {
         // do not ask engine to hide: this avoid a glitch if
         // a picture is simply moved in edit mode

         ui->thumbnail->setVisible( false);
         ui->thumbnail->setPixmap( QPixmap());
      }
      else
      {
         ui->thumbnail->setVisible( true);
         /* create thumbnail of selected picture in upper right corner */
         ui->thumbnail->setPixmap( pixmap.scaled(ui->thumbnail->width(),
                                                 ui->thumbnail->height(),
                                                 Qt::KeepAspectRatioByExpanding));
      }
   }
   else
   {
      /* active media is not a picture */
      ui->thumbnail->setVisible( false);
      ui->thumbnail->setPixmap( QPixmap());
   }
}


void PicturePlaybar::paintEvent(QPaintEvent * event)
{
   /* fix thumbnail position */
   static int containerWidth = static_cast<int>(-1);
   static int parentY = static_cast<int>(-1);

   if ( (m_thumbnailContainer) &&
        ((m_thumbnailContainer->width() != containerWidth) ||
         (parentWidget()->y() != parentY)) )
   {
      ui->thumbnail->move( m_thumbnailContainer->viewport()->width() - THUMBNAIL_SIZE,
                           parentWidget()->y() - THUMBNAIL_SIZE/2);
      containerWidth = m_thumbnailContainer->width();
      parentY = parentWidget()->y();
   }

   QWidget::paintEvent( event);
}

