#ifndef PLAYLISTFUNCTIONFACTORY_H
#define PLAYLISTFUNCTIONFACTORY_H

#include <QObject>

class IF_MediaEngineInterface;
class FullScreenMediaWidget_IF;
class MediaListModel;
class MediaAutomation;
class ActionListController;
class Fader;
class QAction;
class StatusDisplay;
class ApplicationSettings;


class PlaylistFunctionFactory : public QObject
{
   Q_OBJECT
public:
   explicit PlaylistFunctionFactory( QObject *parent = nullptr);

   enum ActionModifier
   {
      BaseActions = 0,
      ShiftedActions  /**< actions are triggered with SHIFT modifier */
   };

   /**
    * @brief build a MediaListModel
    * @param modelTag is used as a tag to identify the model
    * @return MediaListModel
    */
   MediaListModel *buildModel(const QString & modelTag);

   /**
    * @brief buildMediaEngine
    * @param aFader
    * @param displayWidget
    * @param renderModeIndex is an applilcation setting. It is the engine that knows how
    *    to handle such value
    * @return
    */
   IF_MediaEngineInterface * buildMediaEngine( Fader & aFader,
                                               FullScreenMediaWidget_IF & displayWidget,
                                               StatusDisplay & logger,
                                               ApplicationSettings * settings);
   MediaAutomation * buildAutomation(MediaListModel *model, IF_MediaEngineInterface *engine,
                                          ActionListController *controller, Fader *fader,
                                          int defaultVolume, StatusDisplay & msgDisplay);
   QList<QAction *> buildActionList(IF_MediaEngineInterface *mediaEngine,
                                     MediaAutomation *automation,
                                     ActionModifier modifier);
};

#endif // PLAYLISTFUNCTIONFACTORY_H
