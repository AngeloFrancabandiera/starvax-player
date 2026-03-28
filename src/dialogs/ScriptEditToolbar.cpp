#include "ScriptEditToolbar.h"
#include "ui_ScriptEditToolbar.h"
#include "ApplicationIcon.h"
#include "ApplicationSettings.h"

#include <QColorDialog>
#include <QTextCharFormat>
#include <QSignalBlocker>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>


ScriptEditToolbar::ScriptEditToolbar( const ApplicationSettings & settings,
                                      QWidget *parent) :
   QWidget( parent),
   ui(new Ui::ScriptEditToolbar),
   m_settings( settings),
   m_textColor( Qt::black)
{
   ui->setupUi(this);
   ui->colorButton->setStyleSheet("QPushButton#colorButton { background:black}");

   m_alignLeftAction = new QAction( QIcon(IconPath("align-left.png")), "align left", this );
   m_alignCenterAction = new QAction( QIcon(IconPath("align-center.png")), "align center", this );
   m_alignJustifyAction = new QAction( QIcon(IconPath("align-justify.png")), "justify", this );
   m_alignRightAction = new QAction( QIcon(IconPath("align-right.png")), "align right", this );

   m_alignLeftAction->setCheckable( true);
   m_alignCenterAction->setCheckable( true);
   m_alignJustifyAction->setCheckable( true);
   m_alignRightAction->setCheckable( true);

   QActionGroup *alignGroup = new QActionGroup(this);
   connect( alignGroup, &QActionGroup::triggered,
            this, & ScriptEditToolbar::onAlignActionTriggered);

   alignGroup->addAction( m_alignLeftAction);
   alignGroup->addAction( m_alignCenterAction);
   alignGroup->addAction( m_alignJustifyAction);
   alignGroup->addAction( m_alignRightAction);

   QSize iconSize = QSize(m_settings.getIconSize(), m_settings.getIconSize());

   QToolBar * toolbar = new QToolBar( this);
   toolbar->addActions( alignGroup->actions());
   toolbar->setIconSize( iconSize);
   ui->alignActionsLayout->addWidget( toolbar);

   QToolBar * editToolbar = new QToolBar( this);
   editToolbar->setIconSize( iconSize);
   m_boldAction = new QAction( tr("B"), this);
   m_italicAction = new QAction( tr("I"), this);
   m_underlineAction = new QAction( tr("U"), this);
   m_undoAction = new QAction( QIcon(IconPath("undo.png")), tr("undo"), this);
   m_redoAction = new QAction( QIcon(IconPath("redo.png")), tr("redo"), this);

   QFont font;
   font.setPixelSize(m_settings.getIconSize() * 2 / 3);
   m_boldAction->setToolTip("bold");
   font.setBold(true);
   m_boldAction->setFont( font);
   font.setBold(false);
   font.setItalic( true);
   m_italicAction->setToolTip("italic");
   m_italicAction->setFont( font);
   font.setItalic( false);
   m_underlineAction->setToolTip("underline");
   font.setUnderline( true);
   m_underlineAction->setFont( font);

   QAction * copyFormatAction = new QAction( QIcon(IconPath("copy-format.png")), tr("copy format"), this);
   QAction * pasteFormatAction = new QAction( QIcon(IconPath("paste-format.png")), tr("paste format"), this);

   m_undoAction->setEnabled( false);
   m_redoAction->setEnabled( false);

   m_boldAction->setCheckable( true);
   m_italicAction->setCheckable( true);
   m_underlineAction->setCheckable( true);

   editToolbar->addAction( m_boldAction);
   editToolbar->addAction( m_italicAction);
   editToolbar->addAction( m_underlineAction);
   editToolbar->addAction( m_undoAction);
   editToolbar->addAction( m_redoAction);
   editToolbar->addAction( copyFormatAction);
   editToolbar->addAction( pasteFormatAction);
   ui->editActionsLayout->addWidget( editToolbar);

   connect( m_boldAction, & QAction::triggered, this, & ScriptEditToolbar::boldChanged);
   connect( m_italicAction, & QAction::triggered, this, & ScriptEditToolbar::italicChanged);
   connect( m_underlineAction, & QAction::triggered, this, & ScriptEditToolbar::underlineChanged);

   connect( m_undoAction, & QAction::triggered, this, & ScriptEditToolbar::undo);
   connect( m_redoAction, & QAction::triggered, this, & ScriptEditToolbar::redo);
   connect( copyFormatAction, & QAction::triggered, this, & ScriptEditToolbar::copyFormatRequest);
   connect( pasteFormatAction, & QAction::triggered, this, & ScriptEditToolbar::pasteFormatRequest);
}

ScriptEditToolbar::~ScriptEditToolbar()
{
   delete ui;
}

void ScriptEditToolbar::onEditorCurrentCharFormatChanged(const QTextCharFormat & format)
{
   /* change UI according to given format, but avoid to
    * signal che changed state of UI to avoid loops */
   const QSignalBlocker blockFontCombo(ui->fontComboBox);
   const QSignalBlocker blockFontSize(ui->fontSizeSpinbox);
   const QSignalBlocker blockColor(ui->colorButton);

   m_boldAction->setChecked( format.fontWeight() >= QFont::DemiBold);
   m_italicAction->setChecked( format.fontItalic());
   m_underlineAction->setChecked( format.fontUnderline());

   const QStringList & fonts = format.fontFamilies().toStringList();
   if ( ! fonts.isEmpty())
   {
      ui->fontComboBox->setCurrentFont( fonts.first());
   }
   ui->fontSizeSpinbox->setValue( format.font().pointSize());

   QString style = "QPushButton#colorButton { background:%1}";
   QColor col = format.foreground().color();
   ui->colorButton->setStyleSheet( style.arg(col.name()));
}


void ScriptEditToolbar::onAlignmentChanged(Qt::Alignment a)
{
   switch (a)
   {
   case Qt::AlignLeft:
      m_alignLeftAction->setChecked( true);
      break;
   case Qt::AlignCenter:
      m_alignCenterAction->setChecked( true);
      break;
   case Qt::AlignJustify:
      m_alignJustifyAction->setChecked( true);
      break;
   case Qt::AlignRight:
      m_alignRightAction->setChecked( true);
      break;
   default:
      m_alignLeftAction->setChecked( false);
      m_alignCenterAction->setChecked( false);
      m_alignJustifyAction->setChecked( false);
      m_alignRightAction->setChecked( false);
      break;
   }
}

void ScriptEditToolbar::onUndoAvailable(bool available)
{
   m_undoAction->setEnabled( available);
}

void ScriptEditToolbar::onRedoAvailable(bool available)
{
   m_redoAction->setEnabled( available);
}


void ScriptEditToolbar::onAlignActionTriggered(QAction * action)
{
   if (action == m_alignRightAction)
   {
      alignRequest( Qt::AlignRight);
   }
   else if (action == m_alignCenterAction)
   {
      alignRequest( Qt::AlignCenter);
   }
   else if (action == m_alignJustifyAction)
   {
      alignRequest( Qt::AlignJustify);
   }
   else if (action == m_alignLeftAction)
   {
      alignRequest( Qt::AlignLeft);
   }
}

void ScriptEditToolbar::on_fontComboBox_currentFontChanged( const QFont & font)
{
   emit fontFaceChanged( font.family());
}


void ScriptEditToolbar::on_fontSizeSpinbox_valueChanged(int value)
{
   emit fontSizeChanged( value);
}


void ScriptEditToolbar::on_colorButton_clicked()
{
   m_textColor = QColorDialog::getColor( m_textColor);

   /* update button */
   ui->colorButton->setStyleSheet(QString("QPushButton#colorButton { background:%1}").arg(m_textColor.name()));

   /* notify */
   emit fontColorChanged( m_textColor);
}

