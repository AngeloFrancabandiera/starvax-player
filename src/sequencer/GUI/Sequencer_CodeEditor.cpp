#include <QtWidgets>

#include <QCursor>

#include "Sequencer_CodeEditor.h"
#include "StatusDisplay.h"
#include "testableAssert.h"

#include "AbstractMediaSource.h"


namespace Sequencer {

CodeEditor::CodeEditor(QWidget *parent) :
   QPlainTextEdit(parent),
   m_msgDisplay( nullptr),
   m_isEditMode( true)
{
   m_lineNumberArea = new LineNumberArea(this);

   connect( this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
   connect( this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
   connect( this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

   updateLineNumberAreaWidth(0);
   highlightCurrentLine();

   setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded);
   setLineWrapMode( QPlainTextEdit::NoWrap);

   setContextMenuPolicy( Qt::DefaultContextMenu);

   QAction * addRepeatCommand = new QAction("repeat");
   QAction * addEndCommand = new QAction("end");

   QMenu * hallMenu = new QMenu("hall", this);
   QAction * addHallSwitchCommand = new QAction("on/off switch");
   QAction * addHallDimmerCommand = new QAction("dimmerable");

   hallMenu->addAction( addHallSwitchCommand);
   hallMenu->addAction( addHallDimmerCommand);

   QMenu * playlistMenu = new QMenu("playlist", this);
   QAction * addPlaylistPlayCommand = new QAction("play");
   QAction * addPlaylistPauseCommand = new QAction("pause");
   QAction * addPlaylistStopCommand = new QAction("stop");
   QAction * addPlaylistRewindCommand = new QAction("rewind");
   QAction * addPlaylistShowCommand = new QAction("show");
   QAction * addPlaylistHideCommand = new QAction("hide");
   QAction * addPlaylistTopCommand = new QAction("top");
   QAction * addPlaylistBottomCommand = new QAction("bottom");
   QAction * addPlaylistFadeoutCommand = new QAction("fadeout");
   QAction * addPlaylistVolumeCommand = new QAction("volume");
   QAction * addPlaylistWaitCommand = new QAction("wait");

   playlistMenu->addAction( addPlaylistPlayCommand);
   playlistMenu->addAction( addPlaylistPauseCommand);
   playlistMenu->addAction( addPlaylistStopCommand);
   playlistMenu->addAction( addPlaylistShowCommand);
   playlistMenu->addAction( addPlaylistHideCommand);
   playlistMenu->addAction( addPlaylistTopCommand);
   playlistMenu->addAction( addPlaylistBottomCommand);
   playlistMenu->addAction( addPlaylistRewindCommand);
   playlistMenu->addAction( addPlaylistFadeoutCommand);
   playlistMenu->addAction( addPlaylistVolumeCommand);
   playlistMenu->addAction( addPlaylistWaitCommand);

   QMenu * lightMenu = new QMenu("light");
   QAction * addLightTrigger = new QAction("trigger");
   QAction * addLightStop = new QAction("stop");

   lightMenu->addAction( addLightTrigger);
   lightMenu->addAction( addLightStop);

   QMenu * sequencerMenu = new QMenu("sequencer");
   QAction * addSequencePauseCommand = new QAction("pause");
   QAction * addSequenceStopCommand = new QAction("stop");

   sequencerMenu->addAction(addSequencePauseCommand);
   sequencerMenu->addAction(addSequenceStopCommand);

   addAction( addRepeatCommand);
   addAction( addEndCommand);

   addAction( hallMenu->menuAction());
   addAction( playlistMenu->menuAction());
   addAction( sequencerMenu->menuAction());
   addAction( lightMenu->menuAction());

   connect( addRepeatCommand,   & QAction::triggered, this, & CodeEditor::onAddRepeatCommand);
   connect( addEndCommand,      & QAction::triggered, this, & CodeEditor::onAddEndCommand);
   connect( addPlaylistPlayCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistPlayCommand);
   connect( addPlaylistPauseCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistPauseCommand);
   connect( addPlaylistShowCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistShowCommand);
   connect( addPlaylistHideCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistHideCommand);
   connect( addPlaylistTopCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistTopCommand);
   connect( addPlaylistBottomCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistBottomCommand);
   connect( addPlaylistStopCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistStopCommand);
   connect( addPlaylistRewindCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistRewindCommand);
   connect( addPlaylistFadeoutCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistFadeoutCommand);
   connect( addPlaylistVolumeCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistVolumeCommand);
   connect( addPlaylistWaitCommand, & QAction::triggered, this, & CodeEditor::onAddPlaylistWaitCommand);

   connect( addLightTrigger, & QAction::triggered, this, & CodeEditor::onAddLightTriggerCommand);
   connect( addLightStop, & QAction::triggered, this, & CodeEditor::onAddLightStopCommand);
   connect( addSequencePauseCommand, & QAction::triggered, this, & CodeEditor::onAddSequencePauseCommand);
   connect( addSequenceStopCommand, & QAction::triggered, this, & CodeEditor::onAddSequenceStopCommand);

   connect( addHallSwitchCommand,    & QAction::triggered, this, & CodeEditor::onAddHallSwitchCommand);
   connect( addHallDimmerCommand,    & QAction::triggered, this, & CodeEditor::onAddHallDimmerCommand);
}


int CodeEditor::lineNumberAreaWidth()
{
   int digits = 1;
   int max = qMax(1, blockCount());
   while (max >= 10)
   {
      max /= 10;
      ++digits;
   }

   int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

   return space;
}

void CodeEditor::setEditMode(bool isEditMode)
{
   setReadOnly( ! isEditMode);

   if (isEditMode)
   {
      setContextMenuPolicy( Qt::DefaultContextMenu);
   }
   else
   {
      setContextMenuPolicy( Qt::PreventContextMenu);
   }

   /* used to detect behavior on double-click */
   m_isEditMode = isEditMode;
}

void CodeEditor::gotoLineNumber(int lineNumber)
{
   /* block number start from 0 where 'lineNumber' start from 1 */
   QTextCursor cursor(document()->findBlockByLineNumber(lineNumber - 1));
   setTextCursor(cursor);
   highlightCurrentLine();
}

void CodeEditor::attachStatusDisplay( StatusDisplay * msgDisplay)
{
   m_msgDisplay = msgDisplay;
}


void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
   setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
   if (dy)
   {
      m_lineNumberArea->scroll(0, dy);
   }
   else
   {
      m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
   }

   if (rect.contains(viewport()->rect()))
   {
      updateLineNumberAreaWidth(0);
   }
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
   QPlainTextEdit::resizeEvent(e);

   QRect cr = contentsRect();
   m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
   QList<QTextEdit::ExtraSelection> extraSelections;

   QTextEdit::ExtraSelection selection;

   QColor lineColor = QColor(Qt::yellow).lighter( 160);

   selection.format.setBackground( lineColor);
   selection.format.setProperty( QTextFormat::FullWidthSelection, true);
   selection.cursor = textCursor();
   selection.cursor.clearSelection();
   extraSelections.append(selection);

   setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
   QPainter painter(m_lineNumberArea);
   painter.fillRect(event->rect(), Qt::lightGray);


   QTextBlock block = firstVisibleBlock();
   int blockNumber = block.blockNumber();
   int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
   int bottom = top + (int) blockBoundingRect(block).height();

   while (block.isValid() && top <= event->rect().bottom())
   {
      if (block.isVisible() && bottom >= event->rect().top())
      {
         QString number = QString::number(blockNumber + 1);
         painter.setPen(Qt::black);
         painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                          Qt::AlignRight, number);
      }

      block = block.next();
      top = bottom;
      bottom = top + (int) blockBoundingRect(block).height();
      ++blockNumber;
   }
}


void Sequencer::CodeEditor::mouseDoubleClickEvent(QMouseEvent * event)
{
   if (m_isEditMode == true)
   {
      QPlainTextEdit::mouseDoubleClickEvent( event);
   }
   else
   {
      QTextCursor currentCursor= cursorForPosition(event->pos());
      currentCursor.movePosition( QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
      int blockNumber = currentCursor.block().blockNumber();

      /* block number is 0 based, but line number is 1 based */
      emit lineDoubleClicked( blockNumber + 1);
   }
}

void CodeEditor::mousePressEvent(QMouseEvent * event)
{
   if (m_isEditMode)
   {
      /* default behavior */
      QPlainTextEdit::mousePressEvent( event);
   }
   else
   {
      /* do nothing to avoid a wrong feedback on sequence execution */
   }
}



void CodeEditor::dragEnterEvent(QDragEnterEvent *event)
{
   if (event->mimeData()->hasFormat("playlist-data"))
   {
      event->acceptProposedAction();

      if (m_isEditMode && m_msgDisplay)
      {
         m_msgDisplay->showMessage( tr("drop on left for 'play'; drop on right for 'playAll'"));
      }
   }
   else if (event->mimeData()->hasFormat("lightPreset-data"))
   {
      event->acceptProposedAction();
   }
   else if (event->mimeData()->hasFormat("openWebNet-data"))
   {
      event->acceptProposedAction();
   }
   else
   {
      T_ASSERT_REP( m_msgDisplay != nullptr, tr("Fatal: please add a sink to code editor."));
      m_msgDisplay->showMessage( tr("Drop not supported for this object"), StatusDisplay::INFORMATION);
   }
}

void CodeEditor::dragMoveEvent(QDragMoveEvent * /*event*/)
{
   /* do not call base class to let this class accept mime data */
}

void CodeEditor::dropEvent(QDropEvent *event)
{
   if (m_isEditMode)
   {
      handleDropOperation(event);
   }
   else
   {
      T_ASSERT_REP( m_msgDisplay != nullptr, tr("Fatal: please add a sink to code editor."));
      m_msgDisplay->showMessage(tr("Enter <b>edit mode</b> to drop media."), StatusDisplay::WARNING);
   }
}


void CodeEditor::handleDropOperation(QDropEvent *event)
{
   if (event->mimeData()->hasFormat("playlist-data"))
   {
      handleDropOperationForMedia(event);
      event->accept();
   }

   if (event->mimeData()->hasFormat("lightPreset-data"))
   {
      handleDropOperationForLight(event);
      event->accept();
   }

   if (event->mimeData()->hasFormat("openWebNet-data"))
   {
      handleDropOperationForOpenWebNet(event);
      event->accept();
   }
}

void CodeEditor::handleDropOperationForMedia(QDropEvent *event)
{
   QByteArray encodedData = event->mimeData()->data("playlist-data");
   QDataStream stream( & encodedData, QIODevice::ReadOnly);

   QString command;
   QString line;

   /* according to drop point, use play or playAll */
   if (event->position().x() < viewport()->width()/2)
   {
      command = "play";
   }
   else
   {
      command = "playAll";
   }

   AbstractMediaSource::MediaKind kind;
   int kindOverlay;
   QString label;
   QString path; // not used



   while ( ! stream.atEnd())
   {
      stream >> kindOverlay;
      stream >> label;
      stream >> path;  // not used

      kind = static_cast<AbstractMediaSource::MediaKind>( kindOverlay);

      if ((kind == AbstractMediaSource::AUDIO_VIDEO_LINE_A) ||
          (kind == AbstractMediaSource::PICTURE_LINE_A))
      {
         line = "A";
      }
      else
      {
         line = "B";
      }

      /* insert label in quotes */
      textCursor().insertText( QString("playlist %1 %2 \"%3\"\n").arg(line).arg(command).arg(label) );
   }

   event->accept();
}


void CodeEditor::handleDropOperationForLight(QDropEvent *event)
{
   QByteArray encodedData = event->mimeData()->data("lightPreset-data");
   QDataStream stream( & encodedData, QIODevice::ReadOnly);

   QString label;

   while ( ! stream.atEnd())
   {
      stream >> label;
      textCursor().insertText( QString("light trigger \"%1\"\n").arg(label));
   }
}


void CodeEditor::handleDropOperationForOpenWebNet(QDropEvent *event)
{
   QByteArray encodedData = event->mimeData()->data("openWebNet-data");
   QDataStream stream( & encodedData, QIODevice::ReadOnly);

   QString label;
   QString where;
   bool isDimmer;


   while ( ! stream.atEnd())
   {
      stream >> isDimmer;
      stream >> where;
      stream >> label;

      textCursor().insertText( QString("hall on \"%1\" ").arg(label));

      if (isDimmer)
      {
         textCursor().insertText("5");
      }

      textCursor().insertText("\n");
   }
}




void CodeEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    menu->addSeparator();

    foreach( QAction * action, actions())
    {
       menu->addAction( action);
    }

    menu->exec(event->globalPos());
    delete menu;
}

void CodeEditor::onAddRepeatCommand()
{
   textCursor().insertText("repeat ");
}

void CodeEditor::onAddEndCommand()
{
   textCursor().insertText("end ");
}

void CodeEditor::onAddPlaylistPlayCommand()
{
   textCursor().insertText("playlist A play");
}

void CodeEditor::onAddPlaylistPauseCommand()
{
   textCursor().insertText("playlist A pause");
}

void CodeEditor::onAddPlaylistShowCommand()
{
   textCursor().insertText("playlist A show");
}

void CodeEditor::onAddPlaylistHideCommand()
{
   textCursor().insertText("playlist A hide");
}

void CodeEditor::onAddPlaylistTopCommand()
{
   textCursor().insertText("playlist A top");
}

void CodeEditor::onAddPlaylistBottomCommand()
{
   textCursor().insertText("playlist A bottom");
}

void CodeEditor::onAddPlaylistStopCommand()
{
   textCursor().insertText("playlist A stop");
}

void CodeEditor::onAddPlaylistRewindCommand()
{
   textCursor().insertText("playlist A rewind");
}

void CodeEditor::onAddPlaylistFadeoutCommand()
{
   textCursor().insertText("playlist A fadeout");
}

void CodeEditor::onAddPlaylistVolumeCommand()
{
    textCursor().insertText("playlist A volume");
}

void CodeEditor::onAddPlaylistWaitCommand()
{
    textCursor().insertText("playlist A wait");
}

void CodeEditor::onAddLightTriggerCommand()
{
   textCursor().insertText("light trigger ");
}

void CodeEditor::onAddLightStopCommand()
{
   textCursor().insertText("light stop ");
}

void CodeEditor::onAddSequencePauseCommand()
{
   textCursor().insertText("sequence pause ");
}

void CodeEditor::onAddSequenceStopCommand()
{
   textCursor().insertText("sequence stop ");
}

void CodeEditor::onAddHallSwitchCommand()
{
   textCursor().insertText("hall on \"ZONE NAME\" ");
}

void CodeEditor::onAddHallDimmerCommand()
{
   textCursor().insertText("hall on \"ZONE NAME\" 3 ");
}


}  // namespace Sequencer


void Sequencer::CodeEditor::keyPressEvent(QKeyEvent *event)
{
   /* replace tab with 3 spaces */
   if (event->key() == Qt::Key_Tab)
   {
      textCursor().insertText("   ");
      event->accept();
   }
   else if ( (event->key() == Qt::Key_W) && (event->modifiers() & Qt::ControlModifier))
   {
      /* CTRL+D: select word under cursor */
      QTextCursor cursor = textCursor();
      cursor.movePosition( QTextCursor::StartOfWord);
      cursor.movePosition( QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
      setTextCursor( cursor);
      event->accept();
   }
   else
   {
      QPlainTextEdit::keyPressEvent( event);
   }
}



void Sequencer::CodeEditor::focusOutEvent(QFocusEvent * /*event*/)
{
   emit updateEntryListRequest();
}
