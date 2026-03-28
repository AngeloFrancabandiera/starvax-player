#ifndef APPSETTINGSGUI_H
#define APPSETTINGSGUI_H

#include <QDialog>

namespace Ui {
class AppSettingsGui;
}
class ApplicationSettings;

/**
 * @brief The AppSettingsGui is a facility to edit
 *   configuration settings.
 */
class AppSettingsGui : public QDialog
{
   Q_OBJECT

public:
   explicit AppSettingsGui( ApplicationSettings & settings, QWidget *parent = nullptr);
   ~AppSettingsGui();

   // QWidget interface
public slots:
   virtual void setVisible(bool visible);

private slots:
   void on_commitButton_clicked();
   void on_cancelButton_clicked();
   void on_ownFileButton_clicked();

private:
   Ui::AppSettingsGui *ui;
   ApplicationSettings & m_settings;
};

#endif // APPSETTINGSGUI_H
