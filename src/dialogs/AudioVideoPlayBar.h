#ifndef AV_PLAYBAR_H
#define AV_PLAYBAR_H

#include <QGroupBox>
#include "qpushbutton.h"
#include "qlcdnumber.h"
#include "qlabel.h"
#include "IF_MediaEngineInterface.h"

class MediaPositionSlider;
class QIcon;

namespace Ui {
class AudioVideoPlaybar;
}

class AudioVideoPlayBar : public QWidget
{
   Q_OBJECT

public:
   explicit AudioVideoPlayBar(IF_MediaEngineInterface &engine, MediaPositionSlider &seekbar,
                              QWidget *parent = nullptr);
   ~AudioVideoPlayBar() override;

private:
   Ui::AudioVideoPlaybar *ui;
   IF_MediaEngineInterface & m_engine;
   qint64 m_currentTimeMs;

public slots:
   void play();
   void onAudioOnlyChanged( bool audioOnly);

private slots:
   void onTotalTimeChanged(qint64 timeMs );
   void updateTime( qint64 timeMs );
   void onAvSateChanged(MediaObject::AvMediaState newState);

   void on_playButton_clicked();
   void on_pauseButton_clicked();
   void on_stopButton_clicked();
   void on_rewindButton_clicked();

   void on_audioOnlyButton_clicked(bool checked);
   void on_fadeInCheckbox_clicked(bool checked);
   void on_subtitleCheckBox_clicked(bool checked);
   void on_repeatCheckBox_clicked(bool checked);
   void on_frameForwardButton_clicked();
   void on_secondaryButtons_clicked(bool checked);

private:
   void updateTotalTime( qint64 timeMs );
   void setPlaying();
   void setPaused();
   void setStopped();

#if 0
   void showButtons();
   void hideButtons();
#endif
};

#endif // AV_PLAYBAR_H
