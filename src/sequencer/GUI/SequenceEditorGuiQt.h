#ifndef SEQUENCEEDITORGUIQT_H
#define SEQUENCEEDITORGUIQT_H

#include "SequenceEditorGui.h"

namespace Ui {
class SequenceEditorGui;
}

class QWidget;
class SequencerHighlighter;
class StatusDisplay;
class ApplicationSettings;


class SequenceEditorGuiQt : public SequenceEditorGui
{
   Q_OBJECT
public:
   SequenceEditorGuiQt( StatusDisplay & msgDisplay,
                        const ApplicationSettings &settings,
                        QWidget * parent = nullptr);
   ~SequenceEditorGuiQt() override;

   // SequenceEditorGui interface
public:
   QString scriptContent() const override;
   void setEditMode(bool editMode) override;
   void setScriptContent(const QString &content) override;
   void setActiveLine( int line) override;
   void addError(int line, const QString & description) override;
   void clearErrorList() override;
   void setRunningMode( bool isRunning) override;
   void setCodeParsed( bool aligned) override;
   void displayStatusMessage( const QString & msg) override;

private:
   Ui::SequenceEditorGui *ui;
   SequencerHighlighter * m_syntaxHighlighter;
   bool m_isEditMode;
   bool m_isRunning;
   QPixmap * m_stateImage;

private:
   void updateGuiForMode();

private slots:
   void onCodeTextChanged();
};

#endif // SEQUENCEEDITORGUIQT_H
