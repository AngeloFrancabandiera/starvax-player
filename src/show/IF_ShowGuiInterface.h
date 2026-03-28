#ifndef IF_SHOWGUIINTERFACE_H
#define IF_SHOWGUIINTERFACE_H

#include "QString"

class ShowFileParser;

class IF_ShowGuiInterface
{
public:
   IF_ShowGuiInterface() {}
   virtual  ~IF_ShowGuiInterface() {}

   /**
    * select an (existing) show file to open
    * @return Full path of show file
    */
   virtual QString selectShowToOpen() const = 0;

   /**
    * ask user to confirm to load file, even if there might be errors
    * @param errors - a string with errors found during parsing. May be empty.
    * @return true if show must be loaded; false otherwise
    */
   virtual bool confirmShowFileLoad(const QString & errors) const = 0;

   /**
    * select the path and name to save a file
    * @return Full path of show file
    */
   virtual QString selectShowToSave() const = 0;

   /**
    * print an error message to the user. The implementation may be more or less
    * GUI dependent, (message box, console, file log, ...)
    * @param title - a summary or contextualization of the error
    * @param errorString - error details
    */
   virtual void notifyErrorMessage( const QString & title, const QString & errorString) const = 0;

   /**
    * notify the user that show file is being parsed and loaded
    */
   virtual void showLoadingMessage() = 0;

   /**
    * notify user that show loading is complete
    */
   virtual void hideLoadingMessage() = 0;
};

#endif // IF_SHOWGUIINTERFACE_H
