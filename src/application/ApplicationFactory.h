#ifndef APPLICATIONFACTORY_H
#define APPLICATIONFACTORY_H

#include "QObject"

class MainWindow;
class SequenceFunctionFactory;
class ScriptFunctionFactory;

class SequenceRunner;
class SequenceModel;
class ScriptEngine;
class MediaAutomation;
class ShowManager;

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
   void wireScriptFunction(ScriptEngine* scriptEngine,
                            MediaAutomation *avAutomationLineA,
                            MediaAutomation * avAutomationLineB,
                            IF_LightEngineInterface *lightEngine,
                            Sequencer::Functionality *sequencer);
   void wireShowFunction(ShowManager *show, ScriptEngine *scriptEngine);
};

#endif // APPLICATIONFACTORY_H
