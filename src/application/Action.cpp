#include "Action.h"


Action::Action(QObject * parent) :
   QAction( parent),
   m_showInEditMode(true),
   m_showInOperationMode(true)
{
}

Action::Action(const QString & text, QObject * parent) :
   QAction( text, parent),
   m_showInEditMode(true),
   m_showInOperationMode(true)
{
}

Action::Action(const QIcon & icon, const QString & text, QObject * parent) :
   QAction( icon, text, parent),
   m_showInEditMode(true),
   m_showInOperationMode(true)
{
}

Action::~Action()
{
}

void Action::setVisibleInModes(bool inEditMode, bool inOperationMode)
{
   m_showInEditMode = inEditMode;
   m_showInOperationMode = inOperationMode;
}

void Action::bindShortcut(const QKeySequence & shortcut)
{
   QAction::setShortcut( shortcut);

   setToolTip( text().remove('&') + "<p><b><span style=\"color: blue\">" +
               shortcut.toString(QKeySequence::NativeText) +
               "</span></b></p>");
}
