#ifndef ACTIONMODEFILTER_H
#define ACTIONMODEFILTER_H

#include <QObject>
#include <QList>

class QAction;
class Action;


class ActionModeFilter : public QObject
{
   Q_OBJECT
public:
   explicit ActionModeFilter(QObject *parent = nullptr);

   /** append to the list of managed actions  */
   void addActions( const QList<QAction *> & actions);
   void addAction( QAction * action);

public slots:
   /** show/hide actions according to attributes in new mode  */
   void setEditMode( bool editMode);

private:
   QList<Action *> m_actions;
};

#endif // ACTIONMODEFILTER_H
