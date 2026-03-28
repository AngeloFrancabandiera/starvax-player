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
      audVidAction_lineA,
      audVidAction_lineB,
      pictureAction_lineA,
      pictureAction_lineB,
      sequencerEntryAction
   };

public:
   explicit ScriptActionLauncher( ScriptActionTrigger & actionTrigger, QWidget *parent = nullptr);
   ~ScriptActionLauncher();

   void setActionName( const QString &name);
   void setActionType( ActionType type);

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
   QMap< ActionType, QPixmap *> m_actionToIconMap;
   QMap< ActionType, QString> m_actionToLabelMap;

private:
   void activateCurrentAction(bool playImmediately);
   void tryToActivate( bool playImmediately);
};


#endif // SCRIPTACTIONLAUNCHER_H
