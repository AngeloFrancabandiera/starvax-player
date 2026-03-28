#ifndef SEQUENCEEDITORGUI_H
#define SEQUENCEEDITORGUI_H

#include <QObject>


/**
 * @brief The SequenceEditorGui class is an interface for user
 * interaction with the sequence.
 * Note that this is an abstract class that does not depend in Qt GUI.
 */
class SequenceEditorGui : public QObject
{
   Q_OBJECT

public:
   explicit SequenceEditorGui( QObject *parent = nullptr) :
      QObject (parent)
   {}
   virtual ~SequenceEditorGui() {}

   /** @return the content of the script editor */
   virtual QString scriptContent() const = 0;

   /** load a script in editor */
   virtual void setScriptContent( const QString & content) = 0;

   /**
    * in Edit mode, text can be edited and parse button is active, and
    * 'parse' button is active
    * Play and pause button are always active.
    * @param editMode
    */
   virtual void setEditMode( bool editMode) = 0;

   /**
    * highlight a line to indicate that it is executing
    * @param line
    */
   virtual void setActiveLine( int line) = 0;

   /**
    * change GUI according to the fact that sequence is running.
    * This is not correlated to edit mode.
    * @param isRunning: TRUE when sequence is running; FALSE when it's stopped
    */
   virtual void setRunningMode( bool isRunning) = 0;

   /**
    * add an entry to a list of error that is displayed to user
    * @param line
    * @param description
    */
   virtual void addError( int line, const QString & description) = 0;

   /** clear the list of errors for the script */
   virtual void clearErrorList() = 0;

   /**
    * this function gives a visual feedback on aligment between
    * editor content and parsed instruction
    * @param aligned: True when editor is aligned with instructions
    */
   virtual void setCodeParsed( bool aligned) = 0;

   /**
    * show a message to GUI relevant to runtime status of sequencer
    * @param msg is shown for a given period of time or until a new message arrives
    */
   virtual void displayStatusMessage( const QString & msg) = 0;

signals:
   /** 'play' button has been pressed */
   void runSequenceRequest();
   /** 'pause' button has been pressed */
   void pauseSequenceRequest();
   /** 'parse' button has been pressed */
   void parseSequenceRequest();
   /** script content changed */
   void textChanged();
   /** user requested to activate a sequence */
   void activateLineRequest( int line);
};

#endif // SEQUENCEEDITORGUI_H
