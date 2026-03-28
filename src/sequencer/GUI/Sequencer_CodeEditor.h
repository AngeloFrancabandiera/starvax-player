#ifndef SEQUENCER_CODE_EDITOR_H
#define SEQUENCER_CODE_EDITOR_H

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class StatusDisplay;

class LineNumberArea;

namespace Sequencer {


class CodeEditor : public QPlainTextEdit
{
   Q_OBJECT

public:
   CodeEditor( QWidget *parent = nullptr);

   void lineNumberAreaPaintEvent(QPaintEvent *event);
   int lineNumberAreaWidth();

   void setEditMode( bool isEditMode);

   void gotoLineNumber(int lineNumber);

   void attachStatusDisplay(StatusDisplay * msgDisplay);

signals:
   void lineDoubleClicked( int blockNumber);
   void updateEntryListRequest();

protected:
   void resizeEvent(QResizeEvent *event) override;

private slots:
   void updateLineNumberAreaWidth(int newBlockCount);
   void highlightCurrentLine();
   void updateLineNumberArea(const QRect &, int);

   // QWidget interface
protected:
   void mouseDoubleClickEvent(QMouseEvent * event) override;
   void mousePressEvent(QMouseEvent * event) override;
   void dragEnterEvent(QDragEnterEvent *event) override;
   void dragMoveEvent(QDragMoveEvent * event) override;
   void dropEvent(QDropEvent *event) override;
   void contextMenuEvent(QContextMenuEvent *event) override;
   void keyPressEvent(QKeyEvent *event) override;
   void focusOutEvent(QFocusEvent *event) override;

private slots:
   void onAddRepeatCommand();
   void onAddEndCommand();
   void onAddPlaylistPlayCommand();
   void onAddPlaylistPauseCommand();
   void onAddPlaylistShowCommand();
   void onAddPlaylistHideCommand();
   void onAddPlaylistTopCommand();
   void onAddPlaylistBottomCommand();

   void onAddPlaylistStopCommand();
   void onAddPlaylistRewindCommand();
   void onAddPlaylistFadeoutCommand();
   void onAddPlaylistVolumeCommand();
   void onAddPlaylistWaitCommand();
   void onAddLightTriggerCommand();
   void onAddLightStopCommand();
   void onAddSequencePauseCommand();
   void onAddSequenceStopCommand();
   void onAddHallSwitchCommand();
   void onAddHallDimmerCommand();

private:
   StatusDisplay * m_msgDisplay;
   QWidget * m_lineNumberArea;
   bool m_isEditMode;

private:
   void handleDropOperation( QDropEvent *event);
   void handleDropOperationForMedia(QDropEvent *event);
   void handleDropOperationForLight(QDropEvent *event);
   void handleDropOperationForOpenWebNet(QDropEvent *event);

};


class LineNumberArea : public QWidget
{
public:
   LineNumberArea(CodeEditor *editor) : QWidget(editor) {
      codeEditor = editor;
   }

   QSize sizeHint() const override {
      return QSize(codeEditor->lineNumberAreaWidth(), 0);
   }

protected:
   void paintEvent(QPaintEvent *event) override {
      codeEditor->lineNumberAreaPaintEvent(event);
   }

private:
   CodeEditor *codeEditor;
};

}  // namespace Sequencer


#endif  // SEQUENCER_CODE_EDITOR_H
