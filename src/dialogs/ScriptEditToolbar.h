#ifndef SCRIPTEDITTOOLBAR_H
#define SCRIPTEDITTOOLBAR_H

#include <QWidget>
class QTextCharFormat;
class ApplicationSettings;

namespace Ui {
class ScriptEditToolbar;
}

class ScriptEditToolbar : public QWidget
{
   Q_OBJECT

public:
   explicit ScriptEditToolbar( const ApplicationSettings &settings,
                               QWidget *parent = nullptr);
   ~ScriptEditToolbar() override;

   /* used to update toolbar according to current char format */
public slots:
   void onEditorCurrentCharFormatChanged( const QTextCharFormat & format);
   void onAlignmentChanged(Qt::Alignment a);
   void onUndoAvailable( bool available);
   void onRedoAvailable( bool available);

signals:
   void boldChanged( bool isBold);
   void italicChanged( bool isItalic);
   void underlineChanged( bool isUnderline);
   void fontColorChanged( QColor color);
   void fontSizeChanged( int size);
   void fontFaceChanged( QString fontFamily);
   void alignRequest( Qt::AlignmentFlag align);
   void copyFormatRequest();
   void pasteFormatRequest();
   void undo();
   void redo();


private slots:
   void onAlignActionTriggered( QAction * action);
   void on_fontComboBox_currentFontChanged(const QFont &f);
   void on_fontSizeSpinbox_valueChanged(int value);
   void on_colorButton_clicked();

private:
   Ui::ScriptEditToolbar *ui;
   const ApplicationSettings & m_settings;

   QColor m_textColor;

   QAction * m_boldAction;
   QAction * m_italicAction;
   QAction * m_underlineAction;
   QAction * m_alignRightAction;
   QAction * m_alignLeftAction;
   QAction * m_alignCenterAction;
   QAction * m_alignJustifyAction;
   QAction * m_undoAction;
   QAction * m_redoAction;
};

#endif // SCRIPTEDITTOOLBAR_H
