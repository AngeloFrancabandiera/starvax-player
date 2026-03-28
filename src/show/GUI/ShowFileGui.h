#ifndef SHOWFILEGUI_H
#define SHOWFILEGUI_H

#include "IF_ShowGuiInterface.h"

/* forward declaration of a class that holds
 * an actual message viewer.
 */
class MessageContainer;
class ApplicationSettings;
class ShowFileReportDialog;

/**
 * This class implements \a IF_ShowGuiInterface with Qt file
 * dialog boxes and QMessageBox dialogs.
 */
class ShowFileGui : public IF_ShowGuiInterface
{
public:
   ShowFileGui( ApplicationSettings & appSettings);

   /* IF_FileSelectorInterface interface */
public:
   virtual QString selectShowToOpen() const;
   virtual QString selectShowToSave() const;
   virtual bool confirmShowFileLoad(const QString &errors) const;
   virtual void notifyErrorMessage(const QString &title, const QString &errorString) const;
   virtual void showLoadingMessage();
   virtual void hideLoadingMessage();

private:
   QString startPathLocation() const;

private:
   ApplicationSettings & m_appSettings;

   /**
    * wrapper for message viewer. Needed to pass the handler from
    * \a showLoadingMessage to \a hideLoadingMessage
    */
   MessageContainer * m_messageContainer;
   ShowFileReportDialog * m_reportDialog;

};

#endif // SHOWFILEGUI_H
