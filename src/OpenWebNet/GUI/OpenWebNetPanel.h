#ifndef OPENWEBNETPANEL_H
#define OPENWEBNETPANEL_H

#include <QWidget>

class ApplicationSettings;
class OwnModel;
class OpenWebNetEngine_IF;
class StatusDisplay;

namespace Ui {
class OpenWebNetPanel;
}

/**
 * @brief The OpenWebNetPanel class is the main
 * widget placed in Open-Web-Net docked window
 */
class OpenWebNetPanel : public QWidget
{
   Q_OBJECT

public:
   explicit OpenWebNetPanel( QWidget *parent,
                             OwnModel * model,
                             OpenWebNetEngine_IF & ownEngine,
                             ApplicationSettings & settings,
                             StatusDisplay & display);
   ~OpenWebNetPanel();

public slots:
   /** to be called after a configuration file has been loaded.
    * The only purpose is to avoid to see 'modification' flag */
   void onConfigurationFileLoaded();

   void setEditMode( bool editMode);
   void checkToSave();

private slots:
   void onModelChanged();
   void on_saveButton_clicked();
   void on_removeButton_clicked();

private:
   Ui::OpenWebNetPanel *ui;
   OwnModel * m_model;
   ApplicationSettings & m_settings;
   StatusDisplay & m_display;
   bool m_modelChanged;
};

#endif // OPENWEBNETPANEL_H
