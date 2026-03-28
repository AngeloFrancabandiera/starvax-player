#ifndef IF_ACTIONSELECTORINTERFACE_H
#define IF_ACTIONSELECTORINTERFACE_H

#include "QString"

/**
 * This is a GUI independent interface that allows the selection
 * of a script action.
 * Virtual method ::IF_EventSelectorInterface::selectEvent may be
 * implemented via GUI or not.
 */

class IF_ActionSelectorInterface
{
public:

   typedef enum
   {
      NoAction,
      LightAction,
      MediaActionLineA,
      MediaActionLineB,
      PictureActionLineA,
      PictureActionLineB,
      SequenceEntryAction
   } Type;

   IF_ActionSelectorInterface() :
      m_actionType(NoAction)
   {
   }

   virtual void selectEvent( Type actionType) = 0;

   /** retrieve the value set by GUI or manually */
   Type getActionType() const {
      return m_actionType;
   }

   /** manually set action type */
   void setActionType( Type actionType) {
      m_actionType = actionType;
   }

   /** retrieve the value set by GUI or manually */
   QString getActionId() const {
      return m_actionId;
   }

   /** manually set action ID (label) */
   void setActionId( const QString & actionId) {
      m_actionId = actionId;
   }

protected:
   Type m_actionType;
   /** string that identifies the action, given the type */
   QString m_actionId;
};

#endif // IF_ACTIONSELECTORINTERFACE_H
