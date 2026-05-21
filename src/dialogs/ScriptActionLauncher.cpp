#include "ScriptActionLauncher.h"
#include "ui_ScriptActionLauncher.h"

#include "ApplicationIcon.h"
#include "qmessagebox.h"
#include "testableAssert.h"

ScriptActionLauncher::ScriptActionLauncher( ScriptActionTrigger & actionTrigger, QWidget *parent) :
   QWidget(parent),
   m_actionTrigger(actionTrigger),
   ui(new Ui::ScriptActionLauncher),
   m_type(invalidAction)
{
   ui->setupUi(this);

   m_lightPalette.setBrush(QPalette::Window, QColor( 200, 200, 20, 160) );
   m_mediaPaletteA.setBrush(QPalette::Window, QColor( 200, 0, 0, 160) );
   m_mediaPaletteB.setBrush(QPalette::Window, QColor( 0, 0, 200, 160) );
   m_sequencePalette.setBrush(QPalette::Window, QColor( 150, 0, 150, 160) );
   setAutoFillBackground(true);
   ui->playButton->setIcon( QIcon(IconPath("track_play.png")));
   ui->lineLabel->setStyleSheet("color: blue;"
                                "background-color: yellow;");

   m_actionToIconMap[invalidAction] = new QPixmap();
   m_actionToIconMap[lightAction] = new QPixmap("light.png");
   m_actionToIconMap[audioVidAction] = new QPixmap("sound_A.png");
   m_actionToIconMap[pictureAction] = new QPixmap("picture_A.png");
   m_actionToIconMap[sequencerEntryAction] = new QPixmap("sequencer_play.png");
}

ScriptActionLauncher::~ScriptActionLauncher()
{
   delete ui;
}

void ScriptActionLauncher::setActionName(const QString &name)
{
   ui->actionName->setText( name );
}

void ScriptActionLauncher::setActionType( ActionType type, const QString & param)
{
   m_type = type;
   m_param = param;

//   ui->iconLabel->setPixmap( * m_actionToIconMap.value( type, m_actionToIconMap[invalidAction]));  _TODO
   ui->lineLabel->setText( m_param);

   if (type == audioVidAction)
   {
      ui->activateButton->setEnabled(true);
      setPalette( m_mediaPaletteA);
      ui->actionName->setStyleSheet( "QLabel { color: yellow; background-color: red; }" );
   }
   else if (type == pictureAction)
   {
      ui->activateButton->setEnabled(true);
      setPalette( m_mediaPaletteA);
      ui->actionName->setStyleSheet( "QLabel { color: yellow; background-color: red; }" );
   }
   else if (type == lightAction)
   {
      ui->activateButton->setEnabled(true);
      setPalette( m_lightPalette);
      ui->actionName->setStyleSheet( "QLabel { color: purple; background-color: orange; }" );
   }
   else if (type == sequencerEntryAction)
   {
      ui->activateButton->setEnabled(true);
      setPalette( m_sequencePalette);
      ui->actionName->setStyleSheet( "QLabel { color: magenta; background-color: green; }" );
   }
   else
   {
      setPalette( QPalette());
      ui->activateButton->setEnabled(false);
      ui->actionName->setStyleSheet("");
   }
}

/**
 * activate button callback
 */
void ScriptActionLauncher::on_activateButton_clicked()
{
   tryToActivate( false);
}

/**
 * play button callback
 */
void ScriptActionLauncher::on_playButton_clicked()
{
   tryToActivate( true);
}

void ScriptActionLauncher::tryToActivate(bool playImmediately)
{
   static const QString labels[] = {"nullptr",      // invalidAction
                                    "light",        // lightAction
                                    "media",        // audioVidAction
                                    "picture",      // pictureAction
                                    "seq. entry"    // sequencerEntryAction
                                   };
   T_ASSERT( m_type <= sequencerEntryAction);

   try
   {
      activateCurrentAction( playImmediately);
   }
   catch( QString & err)
   {
      QMessageBox::warning( this, tr("Error executin script action"),
                            tr("Error executing %1 action:\n  %2").
                            arg(labels[m_type]).
                            arg(err) );
   }
}

/**
 * emit a signal to let system activate an action
 */
void ScriptActionLauncher::activateCurrentAction(bool playImmediately)
{
   switch (m_type)
   {
   default:
   case invalidAction:
      break;

   case lightAction:
      emit m_actionTrigger.activateLight( ui->actionName->text(), playImmediately );
      break;

   case audioVidAction:
   case pictureAction:
      emit m_actionTrigger.activateMedia( ui->actionName->text(), m_param, playImmediately );
      break;

   case sequencerEntryAction:
      emit m_actionTrigger.activateSequenceEntry( ui->actionName->text(), playImmediately );
      break;
   }
}

