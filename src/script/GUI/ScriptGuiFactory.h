#ifndef SCRIPTGUIFACTORY_H
#define SCRIPTGUIFACTORY_H

#include <array>
#include <QObject>

class IF_ScriptViewerInterface;
class QWidget;
class QStringListModel;
class QAbstractListModel;

class ScriptActionTrigger;
class StatusDisplay;
class ApplicationSettings;


class ScriptGuiFactory : public QObject
{
public:
   ScriptGuiFactory( QWidget *parent);

   IF_ScriptViewerInterface * build( QWidget *container,
                                     ScriptActionTrigger *actionTrigger,
                                     std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & mediaModelSet,
                                     QStringListModel& sequenceEntryModel,
                                     StatusDisplay * msgDisplay,
                                     ApplicationSettings &settings,
                                     QAbstractListModel *lightModel);
};

#endif // SCRIPTGUIFACTORY_H
