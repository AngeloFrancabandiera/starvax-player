#ifndef SEQUENCER_FUNCTIONALITY_H
#define SEQUENCER_FUNCTIONALITY_H

#include <QObject>
#include <QStringListModel>

class SequenceEditorGui;

namespace Sequencer {
class ScriptSequencer;
class Runner;
class ScriptParser;

/** this class is the access point of sequencer functionality
 * for the rest of the application.
 * This is the entry point for commands sent by GUI (except the ones
 * related to writing the script itself).
 */
class Functionality : public QObject
{
   Q_OBJECT
public:
   Functionality( SequenceEditorGui & gui,
                  ScriptSequencer & sequencer,
                  Runner & runner,
                  ScriptParser & parser,
                  QObject * parent = nullptr);

public:
   QStringListModel & entryList() {
      return m_entryLabelsModel;
   }
public slots:
   /** start the execution from current instruction.
    * If current instruction is 'stop', nothing is done */
   void resume();

   /** stop the execution of the script */
   void stop();

   /** set active instruction.
    * @param line is referred to source script
    */
   void setActiveInstructionByLine( int line);

   /**
    * activate the entry instruction for the given "label"
    * @param label
    * @param resume: when true, start the sequence immediately
    */
   void activateEntry( const QString & label, bool resume);

   /**
    * check the script for syntax and build a list of ordered
    * instructions.
    */
   void parse();

private:
   SequenceEditorGui & m_gui;
   ScriptSequencer & m_sequencer;
   Runner & m_runner;
   ScriptParser & m_parser;

   /* list of entry point for blocks */
   QStringList m_entryLabels;
   QStringListModel m_entryLabelsModel;
   /* each index is relevant to the same index in 'm_entryLabels' */
   QList<int> m_entryLines;

private:
   void reportLineErrors();
   void reportScriptErrors();
   void updateEntryList();
   int lineForEntryWithLabel( const QString & label);
};


}  // namespace Sequencer


#endif // SEQUENCER_FUNCTIONALITY_H
