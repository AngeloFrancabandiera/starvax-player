#include "AppSettingsGui.h"
#include "ui_AppSettingsGui.h"
#include "ApplicationSettings.h"
#include "testableAssert.h"
#include "PlaylistDecks.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QSpinBox>
#include <QSpacerItem>
#include <QGridLayout>


AppSettingsGui::AppSettingsGui( ApplicationSettings & settings,
                                QWidget *parent) :
   QDialog(parent),
   ui(new Ui::AppSettingsGui),
   m_settings( settings)
{
   ui->setupUi(this);
   setWindowTitle(tr("Options"));

   T_ASSERT(ui->groupBox_media->layout());
   T_ASSERT(dynamic_cast<QGridLayout*>(ui->groupBox_media->layout()));
   QGridLayout* media_layout = dynamic_cast<QGridLayout*>(ui->groupBox_media->layout());

   // create settings for decks
   for (int deck=0; deck < NUMBER_OF_MEDIA_DECKS; deck++)
   {
      QLabel *label = new QLabel(QString("Default volume for deck %1").arg(Playlist::toLetter(deck)), ui->groupBox_media);
      m_defaultVolumeSet[deck] = new QSpinBox( ui->groupBox_media);
      QSpacerItem *spacer = new QSpacerItem( 10, 10, QSizePolicy::Expanding);

      media_layout->addWidget( label, deck+3, 0);
      media_layout->addWidget( m_defaultVolumeSet[deck], deck+3, 1);
      media_layout->addItem( spacer, deck+3, 2);

      m_defaultVolumeSet[deck]->setMinimum(0);
      m_defaultVolumeSet[deck]->setMaximum(100);
   }
}

AppSettingsGui::~AppSettingsGui()
{
   delete ui;
}


void AppSettingsGui::setVisible(bool visible)
{
   if (visible == true)
   {
      /* upload GUI */
      for (int deck=0; deck < NUMBER_OF_MEDIA_DECKS; deck++)
      {
         m_defaultVolumeSet[deck]->setValue( m_settings.defaultVolume( deck));
      }

      ui->passwordLine->setText( m_settings.remotePassword());
      ui->DmxChannelsSpinBox->setValue( m_settings.numberOfDmxChannels());

      ApplicationSettings::OpenWebNetSettings ownSetting =
          m_settings.getOpenWebNetSettings();
      ui->OwnServerEdit->setText( ownSetting.serverIpAddress);
      ui->OwnFileEdit->setText( ownSetting.configFilePath);

      ui->panelSizeSpinbox->setValue( m_settings.getSidePanelsFontSize());
      ui->editorSizeSpinbox->setValue( m_settings.getTextEditorFontSize());
      ui->iconSizeSpinBox->setValue( m_settings.getIconSize());
      ui->playbackStepSpin->setValue( static_cast<double>(m_settings.playbackStep()) / 1000.0);
      ui->activateNextCheckBox->setChecked( m_settings.activateNextAfterPLayback());

      ui->saveScriptFontSizeCheckBox->setChecked( m_settings.getSaveFontSize());
   }

   QDialog::setVisible( visible);
}


void AppSettingsGui::on_commitButton_clicked()
{
   /* update settings */
   for (int deck=0; deck < NUMBER_OF_MEDIA_DECKS; deck++)
   {
      m_settings.setDefaultVolume( deck, m_defaultVolumeSet[deck]->value());
   }

   m_settings.setRemotePassword( ui->passwordLine->text());
   m_settings.setNumberOfDmxChannels( ui->DmxChannelsSpinBox->value());

   ApplicationSettings::OpenWebNetSettings ownSetting;
   ownSetting.serverIpAddress = ui->OwnServerEdit->text();
   ownSetting.configFilePath = ui->OwnFileEdit->text();

   m_settings.storeOpenWebNetSettings( ownSetting);

   m_settings.storeSidePanelsFontSize( ui->panelSizeSpinbox->value());
   m_settings.storeTextEditorFontSize( ui->editorSizeSpinbox->value());
   m_settings.storeIconSize(ui->iconSizeSpinBox->value());

   m_settings.setPlaybackStep( static_cast<int>(ui->playbackStepSpin->value() * 1000.0));
   m_settings.setActivateNextAfterPLayback( ui->activateNextCheckBox->isChecked());
   m_settings.setSaveFontSize( ui->saveScriptFontSizeCheckBox->isChecked());

   this->accept();
}

void AppSettingsGui::on_cancelButton_clicked()
{
   this->reject();
}

void AppSettingsGui::on_ownFileButton_clicked()
{
   QString oldPath = m_settings.getOpenWebNetSettings().configFilePath;

   QString newPath = QFileDialog::getOpenFileName( nullptr, tr("Select OpenWenNet file ..."),
                                                   QFileInfo(oldPath).absolutePath(),
                                                   "Open Web Net (*.light)");

   if (newPath != QString())
   {
      ui->OwnFileEdit->setText( newPath);
   }
}
