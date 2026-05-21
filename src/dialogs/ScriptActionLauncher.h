#ifndef SCRIPTACTIONLAUNCHER_H
#define SCRIPTACTIONLAUNCHER_H

#include <QWidget>
#include "ScriptActionTrigger.h"
#include <QMap>

namespace Ui {
class ScriptActionLauncher;
}

class ScriptActionLauncher : public QWidget
{
   Q_OBJECT

public:
   enum ActionType
   {
      invalidAction,
      lightAction,
      audioVidAction,
      pictureAction,
      sequencerEntryAction
   };

public:
   explicit ScriptActionLauncher( ScriptActionTrigger & actionTrigger, QWidget *parent = nullptr);
   ~ScriptActionLauncher();

   void setActionName( const QString &name);
   void setActionType( ActionType type, const QString & param);

private slots:
   void on_activateButton_clicked();
   void on_playButton_clicked();

private:
   ScriptActionTrigger & m_actionTrigger;
   Ui::ScriptActionLauncher *ui;
   QPalette  m_lightPalette;
   QPalette  m_mediaPaletteA;
   QPalette  m_mediaPaletteB;
   QPalette  m_sequencePalette;
   enum ActionType m_type;
   QString m_param;
   QMap< ActionType, QPixmap *> m_actionToIconMap;

private:
   void activateCurrentAction(bool playImmediately);
   void tryToActivate( bool playImmediately);
};


#endif // SCRIPTACTIONLAUNCHER_H
