#ifndef SCRIPTFUNCTIONFACTORY_H
#define SCRIPTFUNCTIONFACTORY_H

#include <array>
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

   ScriptEngine * build( QWidget *container,
                         std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & mediaModelSet,
                         QStringListModel& sequenceEntryModel,
                         StatusDisplay * msgDisplay, ApplicationSettings & settings,
                         QAbstractListModel *lightModel);

private:
   QList<QAction *> createEngineActions( ScriptEngine *engine);

private:
   QAction * m_editModeAction;
};

#endif // SCRIPTFUNCTIONFACTORY_H
