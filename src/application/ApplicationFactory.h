#ifndef APPLICATIONFACTORY_H
#define APPLICATIONFACTORY_H

#include <array>
#include <QObject>

class MainWindow;
class SequenceFunctionFactory;
class ScriptFunctionFactory;

class SequenceRunner;
class SequenceModel;
class ScriptEngine;
class MediaAutomation;
class ShowManager;
class StatusDisplay;
class ApplicationSettings;
class FileInport;
class Action;
class ActionModeFilter;

class IF_LightEngineInterface;

namespace Server {
class NetworkInterfaceQt;
class ServerEngine;

}  // namespace Server

namespace Sequencer {
class Functionality;
}


/**
 * Create all objects that have a lifetime as long as
 * the application itself.
 */
class ApplicationFactory : public QObject
{
   Q_OBJECT
public:
   ApplicationFactory();
   ~ApplicationFactory() override;

   MainWindow *mainWindow() {
      return m_mainWindow;
   }

private:
   MainWindow *m_mainWindow;
   ScriptFunctionFactory *m_scriptFactory;

private:
   void build_playlist_function( StatusDisplay * statusDisplay,
                                 ApplicationSettings *applicationSettings,
                                 FileInport *fileInport,
                                 Action * setEditModeAction,
                                 ActionModeFilter * actionMode,
                                 Action * playAllAction );
   void wireScriptFunction( ScriptEngine* scriptEngine,
                            std::array<MediaAutomation *, NUMBER_OF_MEDIA_DECKS> & mediaAutomationSet,
                            IF_LightEngineInterface *lightEngine,
                            Sequencer::Functionality *sequencer);
   void wireShowFunction(ShowManager *show, ScriptEngine *scriptEngine);
};

#endif // APPLICATIONFACTORY_H
