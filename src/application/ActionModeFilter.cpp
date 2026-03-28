#include "ActionModeFilter.h"
#include "Action.h"

ActionModeFilter::ActionModeFilter(QObject *parent) :
   QObject(parent)
{
}

void ActionModeFilter::addActions(const QList<QAction *> &actions)
{
   foreach( QAction * qAction, actions)
   {
      Action * modeAction = dynamic_cast<Action *>(qAction);

      if (modeAction != nullptr)
      {
         m_actions << modeAction;
      }
   }
}

void ActionModeFilter::addAction(QAction *action)
{
   Action * modeAction = dynamic_cast<Action *>(action);

   if (modeAction != nullptr)
   {
      m_actions << modeAction;
   }
}

void ActionModeFilter::setEditMode(bool editMode)
{
   foreach (Action * action, m_actions)
   {
      if (editMode)
      {
         action->setVisible( action->isVisibleInEditMode());
      }
      else  /* operation mode */
      {
         action->setVisible( action->isVisibleInOperationMode());
      }
   }
}


