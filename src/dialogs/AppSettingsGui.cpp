#include "AppSettingsGui.h"
#include "ui_AppSettingsGui.h"
#include "ApplicationSettings.h"

#include <QFileDialog>
#include <QFileInfo>


AppSettingsGui::AppSettingsGui( ApplicationSettings & settings,
                                QWidget *parent) :
   QDialog(parent),
   ui(new Ui::AppSettingsGui),
   m_settings( settings)
{
   ui->setupUi(this);
   setWindowTitle(tr("Options"));
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
      ui->defaultVolumeLineA->setValue( m_settings.defaultVolumeLineA());
      ui->defaultVolumeLineB->setValue( m_settings.defaultVolumeLineB());
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
   m_settings.setDefaultVolumeLineA( ui->defaultVolumeLineA->value());
   m_settings.setDefaultVolumeLineB( ui->defaultVolumeLineB->value());
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
