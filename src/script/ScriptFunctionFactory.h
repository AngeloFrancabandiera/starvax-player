#ifndef SCRIPTFUNCTIONFACTORY_H
#define SCRIPTFUNCTIONFACTORY_H

#include "qobject.h"

class QWidget;
class QAction;
class QAbstractListModel;
class QStringListModel;

class ScriptEngine;
class StatusDisplay;
class ApplicationSettings;


class ScriptFunctionFactory : public QObject
{
   Q_OBJECT
public:
   ScriptFunctionFactory( QAction * editModeAction, QObject *parent);

   ScriptEngine * build(QWidget *container, QAbstractListModel *mediaModelLineA,
                         QAbstractListModel * mediaModelLineB,
                         QStringListModel& sequenceEntryModel,
                         StatusDisplay * msgDisplay, ApplicationSettings & settings,
                         QAbstractListModel *lightModel);

private:
   QList<QAction *> createEngineActions( ScriptEngine *engine);

private:
   QAction * m_editModeAction;
};

#endif // SCRIPTFUNCTIONFACTORY_H
