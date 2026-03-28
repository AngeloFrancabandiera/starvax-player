#include "LightFunctionFactory.h"

#include "LightPresetFactory.h"
#include "LightPresetModel.h"
#include "LightEngine.h"
#include "lighttransitionengine.h"

LightFunctionFactory::LightFunctionFactory(QObject *parent) :
   QObject(parent)
{
}

IF_LightTransitionEngine *LightFunctionFactory::buildTransitionEngine( int numberOfChannels)
{
   LightTransitionEngine *transitionEngine = new LightTransitionEngine( numberOfChannels, this);

   return transitionEngine;
}

LightPresetFactory *LightFunctionFactory::buildPresetFactory()
{
   return new LightPresetFactory();
}

LightPresetModel *LightFunctionFactory::buildModel( int numberOfChannels,
                                                    LightPresetFactory &presetFactory)
{
   LightPresetModel *model = new LightPresetModel( numberOfChannels, &presetFactory, this);

   return model;
}

IF_LightEngineInterface *LightFunctionFactory::buildLightEngine( LightPresetModel & model,
                                                                 IF_LightTransitionEngine & transitionEngine,
                                                                 ActionListController & lightViewController,
                                                                 StatusDisplay & msgDisplay,
                                                                 DmxInterface & dmxInterface)
{
   LightEngine *engine = new LightEngine( model, transitionEngine, lightViewController,
                                          dmxInterface, msgDisplay, this);

   return engine;
}
