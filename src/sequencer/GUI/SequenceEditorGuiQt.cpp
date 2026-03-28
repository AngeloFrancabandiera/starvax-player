#include "SequenceEditorGuiQt.h"
#include "ui_SequenceEditorGui.h"
#include <QCommonStyle>


#include "SequencerHighlighter.h"
#include "testableAssert.h"
#include "ApplicationIcon.h"
#include "ApplicationSettings.h"

#define PLAY_BUTTON_ICON_PATH   IconPath("sequencer_play.png")
#define PAUSE_BUTTON_ICON_PATH  IconPath("sequencer_pause.png")


SequenceEditorGuiQt::SequenceEditorGuiQt( StatusDisplay &msgDisplay,
                                          const ApplicationSettings & settings, QWidget *parent) :
  SequenceEditorGui(parent),
   ui(new Ui::SequenceEditorGui),
   m_isEditMode( true),
   m_isRunning( false)
{
   ui->setupUi(parent);
   ui->sequencerEdit->attachStatusDisplay( & msgDisplay);
   ui->errorTableWidget->hide();

   m_syntaxHighlighter = new SequencerHighlighter( settings, ui->sequencerEdit->document());

   updateGuiForMode();

   ui->runButton->setIcon( QIcon( QString( PLAY_BUTTON_ICON_PATH )));
   ui->pauseButton->setIcon( QIcon( QString( PAUSE_BUTTON_ICON_PATH )));
   ui->parseButton->setIcon( QIcon( QString(":/images/HardCodedIcons/automatic.png")));
   ui->statusMsgLabel->setTextFormat( Qt::RichText);

   /* try automatic build */
   ui->parseButton->setVisible( false);

   connect( ui->runButton, & QPushButton::pressed, this, & SequenceEditorGui::runSequenceRequest);
   connect( ui->pauseButton, & QPushButton::pressed, this, & SequenceEditorGui::pauseSequenceRequest);
   connect( ui->parseButton, & QPushButton::pressed, this, & SequenceEditorGui::parseSequenceRequest);
   connect( ui->sequencerEdit, & QPlainTextEdit::textChanged, this, & SequenceEditorGui::textChanged);

   connect( ui->sequencerEdit, & Sequencer::CodeEditor::lineDoubleClicked,
            this, & SequenceEditorGui::activateLineRequest);
   connect( ui->sequencerEdit, & Sequencer::CodeEditor::updateEntryListRequest,
            this, & SequenceEditorGui::parseSequenceRequest);

   connect( ui->errorTableWidget, & SequencerErrorTableWidget::highlighLineRequest,
            ui->sequencerEdit, & Sequencer::CodeEditor::gotoLineNumber );

   QCommonStyle style;
   m_stateImage = new QPixmap( style.standardIcon(QStyle::SP_MessageBoxWarning).pixmap(16,16));

   /* on startup, assume code not aligned */
   setCodeParsed( false);

   connect( ui->sequencerEdit, & Sequencer::CodeEditor::textChanged,
            this, & SequenceEditorGuiQt::onCodeTextChanged);
}

SequenceEditorGuiQt::~SequenceEditorGuiQt()
{
   delete ui;
}


QString SequenceEditorGuiQt::scriptContent() const
{
   return ui->sequencerEdit->toPlainText();
}

void SequenceEditorGuiQt::setEditMode(bool editMode)
{
   m_isEditMode = editMode;
   ui->sequencerEdit->setEditMode( editMode);
   updateGuiForMode();

   displayStatusMessage("");

   /* code is automatically parsed when switching from
    * edit mode to show mode */
   if (editMode == false)
   {
      parseSequenceRequest();

      /* set current line as active */
      activateLineRequest( 1);
   }
}


void SequenceEditorGuiQt::setScriptContent(const QString &content)
{
   ui->sequencerEdit->setPlainText( content);

   /* when new code is loaded, try to parse it */
   parseSequenceRequest();
}

void SequenceEditorGuiQt::setActiveLine( int line)
{
   ui->sequencerEdit->gotoLineNumber( line);
}


void SequenceEditorGuiQt::addError(int line, const QString & description)
{
   ui->errorTableWidget->addError( line, description);
}

void SequenceEditorGuiQt::clearErrorList()
{
   ui->errorTableWidget->clearErrorList();
}

void SequenceEditorGuiQt::setRunningMode( bool isRunning)
{
   m_isRunning = isRunning;
   updateGuiForMode();
}

void SequenceEditorGuiQt::setCodeParsed( bool aligned)
{
   if (aligned == false)
   {
      T_ASSERT( m_stateImage != nullptr);
      ui->warningLabel->setPixmap( *m_stateImage);
   }
   else
   {
      ui->warningLabel->setPixmap( QPixmap());
   }
}

void SequenceEditorGuiQt::displayStatusMessage(const QString & msg)
{
   ui->statusMsgLabel->setText( msg);
}


/* enable/disable GUI elements for edit and run modes */
void SequenceEditorGuiQt::updateGuiForMode()
{
   if (m_isEditMode)
   {
      ui->parseButton->setEnabled( true);
      ui->runButton->setEnabled( false);
      ui->pauseButton->setEnabled( false);
   }
   else
   {
      ui->parseButton->setEnabled( false);
      ui->runButton->setEnabled( ! m_isRunning);
      ui->pauseButton->setEnabled( m_isRunning);
   }
}

void SequenceEditorGuiQt::onCodeTextChanged()
{
   setCodeParsed( false);
}

