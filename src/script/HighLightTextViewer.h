#ifndef HIGHLIGHTTEXTVIEWER_H
#define HIGHLIGHTTEXTVIEWER_H

#include <QTextBrowser>
#include <QTextBlock>
#include <qtextcursor.h>
#include <QMouseEvent>
#include <qtimer.h>

#include "AbstractMediaSource.h"
class StatusDisplay;
class ApplicationSettings;

/**
 * @brief The HighLightTextViewer class customize the behavoiur
 * of \a QTextBrowser, adding the 'script automation' behavoiur by
 * implementing many functions from \a IF_ScriptViewerInterface.
 * This class can be used by concrete children of \a IF_ScriptViewerInterface
 * to implement such interface.
 */

class HighLightTextViewer : public QTextBrowser
{
   Q_OBJECT
public:
   explicit HighLightTextViewer( StatusDisplay & msgDisplay,
                                 ApplicationSettings & settings,
                                 QWidget *parent = nullptr);

public slots:
   void resetStyle();
   void setCursorColor( QColor color);
   void setCursorBold( bool bold);
   void setCursorItalic( bool italic);
   void setCursorUnderline( bool underline);
   void setCursorFontFamily( QString family);
   void setCursorFontSize( int pointSize);
   void onAlignRequest( Qt::AlignmentFlag align);
   void onCopyFormatRequest();
   void onPasteFormatRequest();

public:
   void onActivateNextBlock();
   int getActiveBlockLength() const;
   void getScriptContent(QString &content) const ;
   void setScriptContent(const QString &content);
   void setEditMode(bool);

   /** read the word under cursor; then search all occourrences
    * of that word and apply a format for script characters */
   void markWordUnderCursorAsCharacter();

public:
   virtual void setHtml( const QString & text );

signals:
   void filteredMouseMoveEvent( const QPoint &);

   // events creaed from drag n drop operation
   void insertPlaylistEventRequest( AbstractMediaSource::MediaKind kind,
                                    const QString & label);
   void insertLightEventRequest( const QString & label);
   void insertSequenceEntryRequest( const QString & label);

   void loadShowRequest( const QString & showPath);

   /** a feedback that given text was not found */
   void textNotFound();

   void alignmentChanged( Qt::Alignment a);

   // QWidget interface
protected:
   void dragEnterEvent(QDragEnterEvent * event) override;
   void dragMoveEvent(QDragMoveEvent * event) override;
   void dropEvent(QDropEvent * event) override;
   void mouseMoveEvent(QMouseEvent *) override;
   void resizeEvent(QResizeEvent *event) override;
   void mouseDoubleClickEvent(QMouseEvent *) override;
   void wheelEvent(QWheelEvent * event) override;
   void contextMenuEvent(QContextMenuEvent *event) override;

public:
   void highlightActiveBlock();
   void setCurrentBlock(const QTextBlock &block);
   void activeBlockClearHighlight();
   void onImageClicked(QTextCursor &);
   void activateBlock( const QTextBlock & block);
   void scrollToActiveBlock();

   QTextBlock calculateNextBlock();

public slots:
   void onSearchBackwardReq( const QString & text);
   void onSearchForwardReq( const QString & text);

private slots:
   void onFilteredMoveTimerExpired();
   void onTextEditorFontSizeChanged( int ptSize);
   void onCurosrPositionChanged();

private:
   void urlDropEvent(QDropEvent* event);
   void dropFileContent( const QString & filePath);
   void playlistDropEvent(QDropEvent* event);
   void lightDropEvent(QDropEvent* event);
   void textDropEvent(QDropEvent* event);
   void applyDefaultFont();
   void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

private:
   StatusDisplay & m_msgDisplay;
   ApplicationSettings & m_settings;
   QTextCursor  *m_cursor;
   QTextBlock   m_currentBlock;
   bool         m_editMode;

   QColor m_textHighLight;
   QColor m_bgHighLight;
   QFont m_defaultFont;
   QTextCharFormat m_saveFormat;

   /** this timer expires when mouse is moved and then
    *  stopped for a given time
    */
   QTimer  m_filteredMoveTimer;
};

#endif // HIGHLIGHTTEXTVIEWER_H
