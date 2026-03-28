#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include "QWidget"
#include "qlayout.h"

#include "mediaWidgets/swiftslider.h"
#include "testableAssert.h"
#include <QPushButton>
#include <QCommonStyle>
#include <QLabel>

#include "IF_MediaEngineInterface.h"

/** Style of volume value label when volume is 0
  */
#define LABEL_STYLE_ON_MUTED   "QLabel { background-color: #FF0000; border: 1px solid black;}"

/** Style of volume value label when volume is 0
  */
#define LABEL_STYLE_ON_NOT_MUTED   "QLabel { background-color: #BBEECC; border: 1px solid black;}"

/** This widget has a slider and a play/mute toggel button.
  * It's meant to be used by audio engine only.
  */
class VolumeSlider : public QWidget
{
   Q_OBJECT

private:
   SwiftSlider m_slider;
   QPushButton m_muteButton;
   QLabel *m_volumeLabel;

public:
   VolumeSlider( IF_MediaEngineInterface &mediaEngine,
                 Qt::Orientation orientation, QLayout *container) :
      QWidget(container->parentWidget()),
      m_slider( orientation, this),
      m_muteButton( QString(""), this)
   {
      T_ASSERT( container != nullptr);

      m_volumeLabel = new QLabel( this);
      m_volumeLabel->setFont( QFont("helvetica", 12, QFont::Bold));
      m_volumeLabel->setMinimumWidth( 40);
      m_volumeLabel->setMaximumWidth( 40);
      m_volumeLabel->setAlignment( Qt::AlignCenter);
      m_volumeLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
      container->addWidget( m_volumeLabel);

      container->addWidget( &m_muteButton);
      container->setAlignment( &m_muteButton, Qt::AlignHCenter);
      m_muteButton.setMaximumWidth( 30);
      m_muteButton.setMaximumHeight( 30);
      m_muteButton.setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum);
      m_muteButton.setCheckable( true);
      m_muteButton.setChecked( false);

      QCommonStyle style;
      m_muteButton.setIcon(style.standardIcon(QStyle::SP_MediaVolume));
      connect( &m_muteButton, SIGNAL(toggled(bool)), &(mediaEngine), SLOT(setMuted(bool)) );

      container->addWidget( &m_slider);
      container->setAlignment( &m_slider, Qt::AlignHCenter);
      m_slider.setRange( 0, 100);
      m_slider.setMinimumWidth( 30 );
      m_slider.setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

      setValue( mediaEngine.volume());

      connect( &m_slider, SIGNAL(valueChanged(int)), &(mediaEngine), SLOT(setVolume(int)) );
      connect( &mediaEngine, SIGNAL(volumeChanged(int)), this, SLOT(onVolumeChanged(int)) );

      connect( &mediaEngine, SIGNAL(muteStatus(bool)), this, SLOT(onMuteToggled(bool)) );


      /* setup according to initial volume */
      onVolumeChanged( mediaEngine.volume());
   }

   virtual ~VolumeSlider() {}

public slots:
   void setValue(int val)
   {
      m_slider.setValue( val);
   }

private slots:
   void onMuteToggled(bool muted)
   {
      QCommonStyle style;

      if (muted)
      {
         m_muteButton.setIcon( style.standardIcon(QStyle::SP_MediaVolumeMuted) );
         m_muteButton.setChecked( true);
         m_volumeLabel->setStyleSheet( LABEL_STYLE_ON_MUTED );
      }
      else
      {
         m_muteButton.setIcon( style.standardIcon(QStyle::SP_MediaVolume) );
         m_muteButton.setChecked( false);
         m_volumeLabel->setStyleSheet( (m_slider.value() == 0) ?
                                          LABEL_STYLE_ON_MUTED : LABEL_STYLE_ON_NOT_MUTED );
      }
   }

   void onVolumeChanged(int volume)
   {
      setValue( volume);

      m_volumeLabel->setText(QString("%1").arg(volume));

      if (volume == 0)
      {
         m_volumeLabel->setStyleSheet( LABEL_STYLE_ON_MUTED);
      }
      else
      {
         m_volumeLabel->setStyleSheet( m_muteButton.isChecked() ?
                                          LABEL_STYLE_ON_MUTED : LABEL_STYLE_ON_NOT_MUTED );
      }
   }
};

#endif // VOLUMESLIDER_H
