#include "HighLightTextViewer.h"

#include <QApplication>
#include <QScrollBar>
#include <QMimeData>
#include <QFileInfo>
#include <QTextStream>
#include <QMenu>
#include <QRegularExpression>
#include <QStringList>

#include "supported_files.h"
#include "StatusDisplay.h"
#include "ApplicationSettings.h"

/******************************  settings *************************************/

/** @{ active block highlight */
#define  SCRIPT_HIGHLIGHT_BG     QColor(255, 255, 50)
#define  SCRIPT_HIGHLIGHT_TEXT   QColor(200, 200, 255)
/** @} */

/*******************************************************************************/

HighLightTextViewer::HighLightTextViewer( StatusDisplay & msgDisplay,
                                          ApplicationSettings &settings,
                                          QWidget *parent) :
   QTextBrowser(parent),
   m_msgDisplay( msgDisplay),
   m_settings( settings)
{
   m_cursor = new QTextCursor(document());
   m_currentBlock = document()->begin();
   m_editMode  = false;

   setObjectName("scriptEdit");

   resetStyle();
   setUndoRedoEnabled( true );
   setOpenLinks( false);

   setContextMenuPolicy( Qt::DefaultContextMenu);

   /* this avoids a scrollbar flickering problem */
   setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

   setSearchPaths( QStringList(qApp->applicationDirPath() + "/res/images/") );

   connect( & m_filteredMoveTimer, SIGNAL(timeout()),
            this, SLOT(onFilteredMoveTimerExpired()) );

   connect( & settings, SIGNAL(textEditorFontSizeChanged(int)),
            this, SLOT(onTextEditorFontSizeChanged(int)));

   connect( this, & HighLightTextViewer::cursorPositionChanged,
            this, & HighLightTextViewer::onCurosrPositionChanged);

   setAcceptDrops( true);

   /* highlight better selected text */
   QPalette pal = palette();
   pal.setColor(QPalette::Highlight, QColor(Qt::darkRed));
   pal.setColor(QPalette::HighlightedText, QColor(Qt::white));
   setPalette( pal);

   m_defaultFont = document()->defaultFont();
}

void HighLightTextViewer::onActivateNextBlock()
{
   activateBlock( calculateNextBlock());
}

int HighLightTextViewer::getActiveBlockLength() const
{
   return m_currentBlock.length();
}

void HighLightTextViewer::getScriptContent(QString &content) const
{
   content = document()->toHtml();
}

void HighLightTextViewer::setScriptContent(const QString &content)
{
   document()->setHtml( content);
   setCurrentBlock( document()->begin());
   scrollToActiveBlock();

   QTextCursor cursor = textCursor();
   cursor.movePosition( QTextCursor::Start, QTextCursor::MoveAnchor);
   setTextCursor( cursor);
}

/**
 * give a default value to style variables
 */
void HighLightTextViewer::resetStyle()
{
   m_textHighLight = SCRIPT_HIGHLIGHT_TEXT;
   m_bgHighLight   = SCRIPT_HIGHLIGHT_BG;

   /* update view */
   highlightActiveBlock();
}


void HighLightTextViewer::setCursorBold(bool bold)
{
   QTextCharFormat fmt;
   fmt.setFontWeight( bold ? QFont::Bold : QFont::Normal);
   mergeFormatOnWordOrSelection(fmt);
}

void HighLightTextViewer::setCursorItalic(bool italic)
{
   QTextCharFormat fmt;
   fmt.setFontItalic( italic);
   mergeFormatOnWordOrSelection(fmt);
}

void HighLightTextViewer::setCursorUnderline(bool underline)
{
   QTextCharFormat fmt;
   fmt.setFontUnderline( underline);
   mergeFormatOnWordOrSelection(fmt);
}

void HighLightTextViewer::setCursorFontFamily(QString family)
{
   QTextCharFormat fmt;
   fmt.setFontFamilies( QStringList(family));
   mergeFormatOnWordOrSelection(fmt);
}

void HighLightTextViewer::setCursorFontSize(int pointSize)
{
   QTextCharFormat fmt;
   fmt.setFontPointSize( qreal(pointSize));
   mergeFormatOnWordOrSelection(fmt);
}

void HighLightTextViewer::onAlignRequest(Qt::AlignmentFlag align)
{
   setAlignment( align);
}

void HighLightTextViewer::onCopyFormatRequest()
{
   m_saveFormat = currentCharFormat();
}

void HighLightTextViewer::onPasteFormatRequest()
{
   QTextCursor cursor = textCursor();
   cursor.setCharFormat( m_saveFormat);
   setTextCursor( cursor);
}

void HighLightTextViewer::setCursorColor(QColor color)
{
   QTextCursor cursor = textCursor();
   QTextCharFormat format = textCursor().charFormat();
   format.setForeground( color);
   cursor.setCharFormat( format);
   setTextCursor( cursor);
}

void HighLightTextViewer::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
    {
       cursor.select(QTextCursor::WordUnderCursor);
    }

    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}


void HighLightTextViewer::activateBlock( const QTextBlock & block)
{
   /* clear-update-highlight sequence */
   activeBlockClearHighlight();
   setCurrentBlock( block );
   highlightActiveBlock();

   scrollToActiveBlock();
}

void HighLightTextViewer::mouseMoveEvent(QMouseEvent * ev)
{
   if (m_editMode == true)
   {
      // default management
      QTextBrowser::mouseMoveEvent( ev);
   }
   else
   {
      // filter the event to avoid unnecessary CPU workload
      m_filteredMoveTimer.start( 100);
   }
}

void HighLightTextViewer::resizeEvent(QResizeEvent * event)
{
   QTextBrowser::resizeEvent( event );

   /* update scrolling position only in read mode */
   if( m_editMode == false )
   {
      scrollToActiveBlock();
   }
}

void HighLightTextViewer::mouseDoubleClickEvent( QMouseEvent * event )
{
   if( m_editMode == true )
   {
      /* in edit mode, use default behaviour */
      QTextEdit::mouseDoubleClickEvent( event);
   }
   else
   {
      /* 'show mode' behaviour */
      QTextCursor currentCursor= cursorForPosition(event->pos());
      currentCursor.movePosition( QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);

      activateBlock( currentCursor.block());
      /* base handler is not called to avoid a spurious selection */
   }
}

void HighLightTextViewer::contextMenuEvent(QContextMenuEvent *event)
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

/**
 * When parameter \p edit is true, editing text is allowed and
 * background changes to make this evident.
 * If \p edit is false, editor is in 'show mode', with auto-
 * scroll allowed, (if enabled).
 */
void HighLightTextViewer::setEditMode(bool edit)
{
   m_editMode = edit;
   setReadOnly( !edit);

   applyDefaultFont();

   if (edit)
   {
      activeBlockClearHighlight();
   }
   else
   {
      /* highlight block of last edit position */
      activateBlock( this->textCursor().block());
   }
}

void HighLightTextViewer::applyDefaultFont()
{
   m_defaultFont.setPointSize( m_settings.getTextEditorFontSize());
   document()->setDefaultFont( m_defaultFont);
}

void HighLightTextViewer::markWordUnderCursorAsCharacter()
{
   QTextCursor cursor =  this->textCursor();
   int oldPosition = cursor.position();
   cursor.select( QTextCursor::WordUnderCursor);
   QString charName = cursor.selectedText();

   cursor.movePosition( QTextCursor::Start);
   setTextCursor( cursor);

   QTextCharFormat fmt;
   fmt.setFontWeight( QFont::Bold);

   bool found = true;

   cursor.beginEditBlock();

   while (found)
   {
      found = find( charName);

      if (found)
      {
         cursor.mergeCharFormat( fmt);
         mergeCurrentCharFormat( fmt);
      }
   }

   cursor.endEditBlock();

   // go back to old position
   cursor.setPosition( oldPosition);
   setTextCursor( cursor);
}

void HighLightTextViewer::setHtml( const QString & text )
{
   QTextEdit::setHtml(text);

   // set the begin of new file as active block
   delete m_cursor;
   m_cursor = new QTextCursor(document());
   m_currentBlock = document()->begin();

   highlightActiveBlock();
}

void HighLightTextViewer::onFilteredMoveTimerExpired()
{
   m_filteredMoveTimer.stop();

   /* filtered hover event is sent only when not in edit mode */
   if (m_editMode == false)
   {
      QPoint currentMousePosition = mapFromGlobal( QCursor::pos() );
      emit filteredMouseMoveEvent( currentMousePosition);
   }
}

void HighLightTextViewer::onSearchBackwardReq( const QString & text)
{
   /* try to find backword */
   bool found = find( text, QTextDocument::FindBackward);

   if ( ! found)
   {
      QTextCursor cursor = textCursor();
      int currentPosition = cursor.position();

      /* wrap text */
      cursor.movePosition( QTextCursor::End);
      setTextCursor( cursor);

      found = find( text, QTextDocument::FindBackward);

      if ( ! found)
      {
         /* 'text' is not present. Return where we were */
         cursor.setPosition( currentPosition );
         setTextCursor( cursor);

         emit textNotFound();
      }
   }
}


void HighLightTextViewer::onSearchForwardReq( const QString & text)
{
   /* try to find forward */
   bool found = find( text);

   if ( ! found)
   {
      QTextCursor cursor = textCursor();
      int currentPosition = cursor.position();

      /* wrap text */
      cursor.movePosition( QTextCursor::Start);
      setTextCursor( cursor);

      found = find( text);

      if ( ! found)
      {
         /* 'text' is not present. Return where we were */
         cursor.setPosition( currentPosition );
         setTextCursor( cursor);

         emit textNotFound();
      }
   }
}

void HighLightTextViewer::onTextEditorFontSizeChanged(int /*ptSize*/)
{
   applyDefaultFont();
}

void HighLightTextViewer::onCurosrPositionChanged()
{
   emit alignmentChanged( alignment());
}


/**
 * set highlight background for current block
 */
void HighLightTextViewer::highlightActiveBlock()
{
   QList<QTextEdit::ExtraSelection> extraSelections;
   QTextEdit::ExtraSelection selection;

   QColor lineColor(m_bgHighLight);

   selection.format.setBackground(lineColor);
   selection.format.setProperty(QTextFormat::FullWidthSelection, true);
   selection.cursor = QTextCursor(*m_cursor);
   selection.cursor.movePosition( QTextCursor::NextBlock, QTextCursor::KeepAnchor);
   extraSelections.append(selection);

   setExtraSelections(extraSelections);
}

/**
 * update current block and set position accordingly
 */
void HighLightTextViewer::setCurrentBlock(const QTextBlock &block)
{
   m_currentBlock = block;
   m_cursor->setPosition( m_currentBlock.position() );
}

/**
 * remove highlighted background for current block
 */
void HighLightTextViewer::activeBlockClearHighlight()
{
   setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

/**
 *  return the block following to the active one
 */
QTextBlock HighLightTextViewer::calculateNextBlock()
{
   QTextBlock new_block;

   // return next block, if valid
   new_block = m_currentBlock.next();

   if( !(new_block.isValid()) )
   {
      new_block = document()->begin();
   }

   return new_block;
}

/**
 * adjust vertical scrollbar such that active block
 * appares in the middle of viewport
 */
void HighLightTextViewer::scrollToActiveBlock()
{
   int y_wport, yScroll_val, vMin, vMax;

   // scroll to selection
   y_wport = cursorRect( *m_cursor ).top();
   yScroll_val = verticalScrollBar()->value();
   yScroll_val += (y_wport - (viewport()->height() / 2) );

   // check scroll ranges
   vMin = verticalScrollBar()->minimum();
   if( yScroll_val < vMin)
   {
      yScroll_val = vMin;
   }

   vMax = verticalScrollBar()->maximum();
   if( yScroll_val > vMax)
   {
      yScroll_val = vMax;
   }

   verticalScrollBar()->setValue( yScroll_val );
}

void HighLightTextViewer::dragEnterEvent(QDragEnterEvent * event)
{
   if ( (event->mimeData()->hasFormat("playlist-data")) ||
        (event->mimeData()->hasFormat("lightPreset-data")) ||
        (event->mimeData()->hasFormat("sequence-data")) ||
        (event->mimeData()->hasFormat("text/plain")) )
   {
      event->acceptProposedAction();
   }

   else if (event->mimeData()->hasUrls())
   {
      foreach (QUrl url, event->mimeData()->urls())
      {
         QString ext = QFileInfo( url.toLocalFile()).suffix();

         if (SCRIPT_SUPPORTED_FORMATS.contains(ext) ||
             SHOWS_SUPPORTED_FORMATS.contains(ext) )
         {
            event->acceptProposedAction();
         }
      }

      /* visual feedback is given on drop. Here we rely on system cursor shape */
   }
}


void HighLightTextViewer::dropEvent(QDropEvent * event)
{
   setTextCursor( cursorForPosition( event->position().toPoint()) );

   if (event->mimeData()->hasUrls())
   {
      urlDropEvent(event);
   }

   /* add actions in edit mode only */
   if (m_editMode == true)
   {
      if (event->mimeData()->hasFormat("playlist-data"))
      {
         playlistDropEvent(event);
      }

      if (event->mimeData()->hasFormat("lightPreset-data"))
      {
         lightDropEvent(event);
      }

      if (event->mimeData()->hasFormat("text/plain"))
      {
         textDropEvent(event);
      }
   }
   else
   {
      m_msgDisplay.showMessage(tr("Events can be added in <b>Edit Mode</b> only."),
                               StatusDisplay::WARNING);
   }

   event->acceptProposedAction();
}

void HighLightTextViewer::urlDropEvent(QDropEvent* event)
{
   QString showFilePath = QString();
   bool dropRejectedFlag = false;  /* in normal mode, only Show files can be dropped */

   foreach (QUrl url, event->mimeData()->urls())
   {
      QString ext = QFileInfo( url.toLocalFile()).suffix();

      if ((dropRejectedFlag == false) && (SCRIPT_SUPPORTED_FORMATS.contains(ext)))
      {
         if (m_editMode == true)
         {
            event->acceptProposedAction();
            dropFileContent( url.toLocalFile());
         }
         else
         {
            dropRejectedFlag = true;
         }
      }

      if (SHOWS_SUPPORTED_FORMATS.contains(ext))
      {
         /* a show path is handled at the end of all other files */
         event->acceptProposedAction();
         showFilePath = url.toLocalFile();
      }
   }

   if (showFilePath != QString())
   {
      /* load new show */
      emit loadShowRequest( showFilePath);
   }
   else if (dropRejectedFlag)
   {
      m_msgDisplay.showMessage(tr("Enter Edit mode to modify script"), StatusDisplay::WARNING);
   }
}

void HighLightTextViewer::dropFileContent(const QString & filePath)
{
   QFile file( filePath);

   bool res = file.open( QIODevice::ReadOnly);

   if ((res == true) && (file.isOpen()))
   {
      QString suffix = QFileInfo(file).suffix();

      QTextStream stream( & file);
      if ((suffix == "htm") || (suffix == "html"))
      {
         textCursor().insertHtml( stream.readAll());
      }
      else
      {
         textCursor().insertText( stream.readAll());
      }
   }
   else
   {
      m_msgDisplay.showMessage(tr("File <i>%1</i> can't be open for reading.").
                               arg(filePath), StatusDisplay::WARNING);
   }
}

void HighLightTextViewer::playlistDropEvent(QDropEvent* event)
{
   QByteArray encodedData = event->mimeData()->data("playlist-data");
   QDataStream stream( & encodedData, QIODevice::ReadOnly);

   int kind;  // AbstractMediaSource::MediaKind
   QString label;
   QString path; // not used

   while ( ! stream.atEnd())
   {
      stream >> kind;
      stream >> label;
      stream >> path;  // not used

      emit insertPlaylistEventRequest( static_cast<AbstractMediaSource::MediaKind>(kind),
                                       label);
   }
}

void HighLightTextViewer::lightDropEvent(QDropEvent* event)
{
   QByteArray encodedData = event->mimeData()->data("lightPreset-data");
   QDataStream stream( & encodedData, QIODevice::ReadOnly);

   QString label;

   while ( ! stream.atEnd())
   {
      stream >> label;
      emit insertLightEventRequest( label);
   }
}

void HighLightTextViewer::textDropEvent(QDropEvent *event)
{
   QByteArray encodedData = event->mimeData()->data("text/plain");
   QString text = QString::fromLatin1( encodedData);

   static const QRegularExpression seqEntryReg("\\s*entry\\s+\"(.*)\"\\s*:\\s*");

   QRegularExpressionMatch match = seqEntryReg.match( text);

   /* search for special text */
   if (match.hasMatch())
   {
      QString label = match.captured(1);
      emit insertSequenceEntryRequest( label);
   }
   else
   {
      /* just paste text */
      textCursor().insertText( text);
   }
}



void HighLightTextViewer::dragMoveEvent(QDragMoveEvent * /*event*/)
{
   /* do not call base class to let this class accept mime data */
}


void HighLightTextViewer::wheelEvent(QWheelEvent * event)
{
   if (event->modifiers() & Qt::ControlModifier)
   {
      if (event->angleDelta().y() > 0)
      {
         zoomIn();
      }
      else
      {
         zoomOut();
      }
   }
   else
   {
      QTextBrowser::wheelEvent( event);
   }
}

