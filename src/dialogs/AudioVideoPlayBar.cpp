#include "AudioVideoPlayBar.h"
#include "ui_AudioVideoPlayBar.h"
#include <QTime>
#include "qapplication.h"
#include "ApplicationIcon.h"

#include "mediaPositionSlider.h"
#include <QIcon>
#include <QFontMetrics>
#include <QFontDatabase>
#include <QCommonStyle>

#include <QDebug>

#define TICK_INTERVAL_MS  100

#define PLAY_BUTTON_ICON_PATH   IconPath("track_play.png")
#define PAUSE_BUTTON_ICON_PATH  IconPath("track_pause.png")
#define STOP_BUTTON_ICON_PATH  IconPath("track_stop.png")
#define REWIND_BUTTON_ICON_PATH  IconPath("track_rewind.png")
#define AUDIO_ONLY_BUTTON_ICON_PATH  IconPath("picture.png")


AudioVideoPlayBar::AudioVideoPlayBar(IF_MediaEngineInterface &engine, MediaPositionSlider &seekbar,
                           QWidget *parent) :
   QWidget(parent),
   ui(new Ui::AudioVideoPlaybar),
   m_engine(engine),
   m_currentTimeMs(0)
{
   ui->setupUi(this);
   ui->seekBarLayout->addWidget( &seekbar);

   int id = QFontDatabase::addApplicationFont( FontPath("LCD.ttf"));
   QStringList families = QFontDatabase::applicationFontFamilies(id);
   QString family = (families.size() > 0) ? families.at(0) : "courier";

   QFont lcdFont(family);
   lcdFont.setPixelSize(24);
   ui->lcdTime->setFont( lcdFont);
   ui->lcdTime->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);

   ui->lcdTime->setText("00:00.0");
   ui->playButton->setFocusPolicy(Qt::NoFocus);
   ui->pauseButton->setFocusPolicy(Qt::NoFocus);
   ui->stopButton->setFocusPolicy(Qt::NoFocus);
   ui->rewindButton->setFocusPolicy(Qt::NoFocus);

   // fix LCD size
   QFontMetrics metric(lcdFont);
   int width = metric.size( 0, "000:00.0").width();
   ui->lcdTime->setMinimumWidth( width);
   ui->lcdTime->setMaximumWidth( width);

   // load icons
   QIcon * playIcon  = new QIcon( QString( PLAY_BUTTON_ICON_PATH ));
   QIcon * pauseIcon = new QIcon( QString( PAUSE_BUTTON_ICON_PATH ));
   QIcon * stopIcon = new QIcon( QString( STOP_BUTTON_ICON_PATH ));
   QIcon * rewindIcon = new QIcon( QString( REWIND_BUTTON_ICON_PATH ));
   QIcon * audioOnlyIcon = new QIcon( QString(AUDIO_ONLY_BUTTON_ICON_PATH));

   ui->playButton->setIcon( *playIcon);
   ui->pauseButton->setIcon( *pauseIcon);
   ui->stopButton->setIcon( *stopIcon);
   ui->rewindButton->setIcon( *rewindIcon);
   ui->audioOnlyButton->setIcon( *audioOnlyIcon);

   QCommonStyle style;
   ui->frameForwardButton->setIcon(style.standardIcon(QStyle::SP_MediaSeekForward));

   engine.setTickInterval( TICK_INTERVAL_MS);

   // connections
   connect( &engine, & IF_MediaEngineInterface::AvMediaStateChanged,
            this, & AudioVideoPlayBar::onAvSateChanged );
   connect( &engine, & IF_MediaEngineInterface::totalTimeChanged,
            this, & AudioVideoPlayBar::onTotalTimeChanged );
   connect( &engine, & IF_MediaEngineInterface::tick,
            this, & AudioVideoPlayBar::updateTime );

   connect( &engine, & IF_MediaEngineInterface::audioOnlyChanged,
            this, & AudioVideoPlayBar::onAudioOnlyChanged );

   // once used, icons can be deleted
   delete playIcon;
   delete pauseIcon;
   delete stopIcon;
   delete rewindIcon;
   delete audioOnlyIcon;
}

AudioVideoPlayBar::~AudioVideoPlayBar()
{
   delete ui;
}

void AudioVideoPlayBar::play()
{
   m_engine.play();
}

/************************************************/
/* updateTime                                  */
/*  update lcd display from a number of ms      */
/************************************************/
void  AudioVideoPlayBar::updateTime(qint64 timeMs )
{
   qint32 min = (timeMs / 60000) % 60;  // minutes
   qint32 sec = (timeMs / 1000) % 60;   // seconds
   qint32 dec = (timeMs % 1000) / 100;  // tenths of seconds

   ui->lcdTime->setText( QString("%1:%2.%3")
                         .arg(min, 3, 10, QChar(' '))
                         .arg(sec, 2, 10, QChar('0'))
                         .arg(dec, 1, 10));

   m_currentTimeMs = timeMs;
}

/**
 * To be called when duration of current media changes.
 * This include a track change.
 * @param timeMs - time in milliseocnds. When <= 0, no track
 *    is currently activated.
 */
void AudioVideoPlayBar::onTotalTimeChanged(qint64 timeMs)
{
   qDebug() << "total time: " << (double)timeMs/1000. << " s";
   if (timeMs > 0)
   {
      updateTotalTime( timeMs);
   }
}

/************************************************/
/* updateTime                                  */
/*  update total time label from total ms       */
/************************************************/
void  AudioVideoPlayBar::updateTotalTime(qint64 timeMs )
{
   // update total time label
   QTime displayTime( 0, (timeMs / 60000) % 60, (timeMs / 1000) % 60 );

   qDebug() << "total: " << displayTime;

   ui->lbTotalTime->setText( displayTime.toString("mm:ss") );
}


void AudioVideoPlayBar::onAvSateChanged(MediaObject::AvMediaState newState)
{
   switch (newState)
   {
   case MediaObject::LoadingState:
      ui->lcdTime->setText("LOADING...");
      break;

   case MediaObject::LoadedState:
      // rewrite current time. This does NOT happen at time 0 only.
      updateTime(m_currentTimeMs);
      break;

   case MediaObject::BufferingState:
      ui->lcdTime->setText("BUFFERING...");
      break;

   case MediaObject::ErrorState:
      ui->lcdTime->setText("MEDIA ERROR");
      break;
   }
}

void AudioVideoPlayBar::onAudioOnlyChanged( bool audioOnly)
{
   /* update button status to be coherent with new state.
    * This is required because 'audio-only' can be set not only
    * by pressng the button, but also with external keys or
    * commands */
   ui->audioOnlyButton->setChecked( audioOnly);
}

#if 0  // TODO remove
/************************************************/
/* setPlaying                                  */
/*  update graphic elements according to        */
/*  'playing' state                             */
/************************************************/
void  AudioVideoPlayBar::setPlaying()
{
   showButtons();
   ui->playButton->setEnabled( false);
   ui->pauseButton->setEnabled( true);
   ui->audioOnlyButton->setEnabled( true);

   /* fade in is one shot */
   ui->fadeInCheckbox->setChecked( false);
   on_fadeInCheckbox_clicked( false);
}

/************************************************/
/* setPaused                                   */
/*  update graphic elements according to        */
/*  'paused' state                              */
/************************************************/
void  AudioVideoPlayBar::setPaused()
{
   showButtons();
   ui->playButton->setEnabled( true);
   ui->pauseButton->setEnabled( false);
   ui->audioOnlyButton->setEnabled( true);
}

void AudioVideoPlayBar::setStopped()
{
   showButtons();
   ui->playButton->setEnabled( true);
   ui->pauseButton->setEnabled( false);
   ui->audioOnlyButton->setEnabled( true);
}


void AudioVideoPlayBar::showButtons()
{
   ui->playButton->setVisible( true);
   ui->pauseButton->setVisible( true);
   ui->stopButton->setVisible( true);
   ui->rewindButton->setVisible( true);
   ui->audioOnlyButton->setVisible( true);
   ui->subtitleCheckBox->setVisible( true);
   ui->repeatCheckBox->setVisible( true);
   ui->fadeInCheckbox->setVisible( true);
   ui->frameForwardButton->setVisible( true);
   ui->secondaryButtons->setVisible( true);
}

void AudioVideoPlayBar::hideButtons()
{
   ui->playButton->setVisible( false);
   ui->pauseButton->setVisible( false);
   ui->stopButton->setVisible( false);
   ui->rewindButton->setVisible( false);
   ui->audioOnlyButton->setVisible( false);
   ui->subtitleCheckBox->setVisible( false);
   ui->repeatCheckBox->setVisible( false);
   ui->fadeInCheckbox->setVisible( false);
   ui->frameForwardButton->setVisible( false);
   ui->secondaryButtons->setVisible( false);
}
#endif

void AudioVideoPlayBar::on_playButton_clicked()
{
   m_engine.play();
}

void AudioVideoPlayBar::on_pauseButton_clicked()
{
   m_engine.pause();
}

void AudioVideoPlayBar::on_stopButton_clicked()
{
   m_engine.stop();
}

void AudioVideoPlayBar::on_rewindButton_clicked()
{
   m_engine.rewind();
}

void AudioVideoPlayBar::on_audioOnlyButton_clicked( bool checked)
{
   m_engine.setAudioOnly( checked);
   /* don't change UI according to this command.
    * Engine will trigger required changes. */
}

void AudioVideoPlayBar::on_fadeInCheckbox_clicked(bool checked)
{
   m_engine.enableFadeIn( checked);
}


void AudioVideoPlayBar::on_subtitleCheckBox_clicked( bool checked)
{
   if (checked) {
      m_engine.enableSubtitles();
   } else {
      m_engine.disableSubtitles();
   }
}


void AudioVideoPlayBar::on_repeatCheckBox_clicked( bool checked)
{
   m_engine.setLoopPlayback( checked);
}


void AudioVideoPlayBar::on_frameForwardButton_clicked()
{
   m_engine.singleFrameForward();
}

void AudioVideoPlayBar::on_secondaryButtons_clicked(bool checked)
{
   ui->subtitleCheckBox->setVisible( checked);
   ui->repeatCheckBox->setVisible( checked);
   ui->fadeInCheckbox->setVisible( checked);
   ui->frameForwardButton->setVisible( checked);

   if (checked)
   {
      qDebug() << m_currentTimeMs;
   }
}

