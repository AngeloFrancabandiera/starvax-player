#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileInfo>
#include <QApplication>

#include "ShowFileGui.h"
#include "supported_files.h"
#include "ApplicationSettings.h"
#include "ApplicationIcon.h"
#include "ApplicationSettings.h"
#include "ShowFileReportDialog.h"

/**
 * MessageContainer is an helper class that wraps a QMessageBox.
 * It is used not avoid any reference to the specific GUI message
 * viewer in header file.
 */
class MessageContainer
{
public:
   MessageContainer() {}

   QMessageBox messageBox;
};


ShowFileGui::ShowFileGui( ApplicationSettings & appSettings) :
   m_appSettings( appSettings),
   m_messageContainer(nullptr),
   m_reportDialog( new ShowFileReportDialog(nullptr))
{
}


QString ShowFileGui::selectShowToOpen() const
{
   QString filename;
   QString startDir = startPathLocation();

   filename = QFileDialog::getOpenFileName( nullptr, QObject::tr("Open show file"),
                                            startDir, SHOWS_DLG_FILTER );

   return filename;
}

QString ShowFileGui::selectShowToSave() const
{
   QString filename;
   QString startDir = startPathLocation();

   filename = QFileDialog::getSaveFileName( nullptr, QObject::tr("Save show file"),
                                            startDir, SHOWS_DLG_FILTER );

   return filename;
}


bool ShowFileGui::confirmShowFileLoad(const QString &errors) const
{
   if (errors != QString())
   {
      m_reportDialog->setReportDetails( errors);

      /* modal execution */
      m_reportDialog->exec();
   }

   /* file is always loaded. Eventual errors have been fixed or accepted by user */
   return true;
}

/**
 * This implementation of \a IF_ShowGuiInterface::notifyErrorMessage uses a
 * QMessageBox object. \p title and \p errorString become QMessageBox title and message.
 */
void ShowFileGui::notifyErrorMessage(const QString &title, const QString &errorString) const
{
   QMessageBox::warning( nullptr, title, errorString, QMessageBox::Ok);
}

/**
 * guess what is the best folder to start a selection of a file,
 * both for open and save.
 * If there is a 'last save file', its path is used; otherwise a default
 * location is returned.
 *
 * @return - directory path
 */
QString ShowFileGui::startPathLocation() const
{
   QString startPath;
   QString lastShow;

   if ( ! m_appSettings.recentShowFiles().isEmpty())
   {
      lastShow = m_appSettings.recentShowFiles().first();
   }

   if ((lastShow != QString()) && (QFileInfo(lastShow).exists()))
   {
      startPath = QFileInfo(lastShow).absoluteDir().absolutePath();
   }
   else
   {
      startPath = QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).at(0);
   }

   return startPath;
}

/**
 * This function allocates a \a MessageContainer that must
 * be deleted by \a hideLoadingMessage
 */
void ShowFileGui::showLoadingMessage()
{
   if (m_messageContainer != nullptr)
   {
      delete m_messageContainer;
   }
   m_messageContainer = new MessageContainer();

   m_messageContainer->messageBox.setWindowTitle(QObject::tr("please wait ..."));
   m_messageContainer->messageBox.setText(QObject::tr("loading show file ..."));
   m_messageContainer->messageBox.setIconPixmap( QPixmap(IconPath("Starvax.png")) );
   m_messageContainer->messageBox.show();

   /* immediately draw the dialog */
   qApp->processEvents();
}

void ShowFileGui::hideLoadingMessage()
{
   if (m_messageContainer != nullptr)
   {
      m_messageContainer->messageBox.hide();
      delete m_messageContainer;
      m_messageContainer = nullptr;
   }
}

