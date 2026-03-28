#ifndef ACTIONHOVERDETECTION_H
#define ACTIONHOVERDETECTION_H

#include <QObject>
#include "ScriptActionLauncher.h"
#include <QMap>

class QTextBrowser;
class QPoint;
class QTextCursor;

class ActionHoverDetection : public QObject
{
   Q_OBJECT
public:
   explicit ActionHoverDetection(ScriptActionLauncher *actionLauncher, QTextBrowser *parent);
   ~ActionHoverDetection();

public slots:
   void onMouseMoveFiltered( const QPoint &position);

private:
   QTextBrowser *m_viewer;
   ScriptActionLauncher *m_actionLauncher;
   QMap<QString, ScriptActionLauncher::ActionType> m_actionTypeForLabel;

private:
   bool cursorIsOverEventIcon( const QTextCursor & cursor);
   ScriptActionLauncher::ActionType detectActionType(const QStringList &anchorData);
   QString formatActionLabel( ScriptActionLauncher::ActionType type, const QString & fullData);

   // QObject interface
public:
   virtual bool eventFilter(QObject *target, QEvent *event);
};

#endif // ACTIONHOVERDETECTION_H
