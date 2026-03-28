#ifndef LIGHTFUNCTIONFACTORY_H
#define LIGHTFUNCTIONFACTORY_H

#include <QObject>

class LightPresetModel;
class LightPresetFactory;
class IF_LightEngineInterface;
class IF_LightTransitionEngine;
class ActionListController;
class DmxInterface;
class StatusDisplay;

class LightFunctionFactory : public QObject
{
public:
   explicit LightFunctionFactory(QObject *parent = 0);

   IF_LightTransitionEngine *buildTransitionEngine(int numberOfChannels);
   LightPresetFactory *buildPresetFactory();
   LightPresetModel *buildModel(int numberOfChannels, LightPresetFactory &presetFactory);
   IF_LightEngineInterface *buildLightEngine( LightPresetModel & model,
                                              IF_LightTransitionEngine & transitionEngine,
                                              ActionListController & lightViewController,
                                              StatusDisplay & msgDisplay,
                                              DmxInterface & dmxInterface);
};

#endif // LIGHTFUNCTIONFACTORY_H
