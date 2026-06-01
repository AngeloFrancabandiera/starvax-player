#include "PlaylistFunctionFactory.h"

#include <QMap>
#include <QFontDatabase>

#include "Action.h"
#include "MediaEngineImp.h"
#include "MediaListModel.h"
#include "MediaSourceFactory.h"
#include "MediaAutomation.h"
#include "ApplicationIcon.h"
#include "ApplicationSettings.h"
#include "testableAssert.h"

#include "MediaToolbar.h"

namespace  {

struct ActionDescriptor
{
   QString iconPath;
   QString lineLabel;
   QKeySequence keySequence;

   ActionDescriptor( QString aIconPath,
                     QString aLineLabel,
                     QKeySequence aKeySequence) :
      iconPath( aIconPath),
      lineLabel( aLineLabel),
      keySequence( aKeySequence)
   {}
};

const ActionDescriptor NullActionDescriptor = {"","", 0};

}  // namespace


PlaylistFunctionFactory::PlaylistFunctionFactory(QObject *parent) :
   QObject(parent)
{
}

MediaListModel *PlaylistFunctionFactory::buildModel( const QString & modelTag)
{
   MediaSourceFactory * mediaSourceFactory = new MediaSourceFactory();
   MediaListModel *model = new MediaListModel( mediaSourceFactory, this);
   model->setModelTag( modelTag);

   return model;
}


IF_MediaEngineInterface *PlaylistFunctionFactory::buildMediaEngine(Fader & aFader,
                                                                    FullScreenMediaWidget_IF & displayWidget,
                                                                    StatusDisplay & logger,
                                                                   ApplicationSettings *settings)
{
   IF_MediaEngineInterface *engine = new MediaEngineImp( aFader, displayWidget, logger, this);
   engine->setStepSizeMs( settings->playbackStep());
   connect( settings, & ApplicationSettings::playbackStepChanged, engine, & IF_MediaEngineInterface::setStepSizeMs);

   return engine;
}

MediaAutomation *PlaylistFunctionFactory::buildAutomation( MediaListModel *model,
                                                           IF_MediaEngineInterface *engine,
                                                           ActionListController *controller,
                                                           Fader *fader, int defaultVolume,
                                                           StatusDisplay & msgDisplay )
{
   MediaAutomation *automation = new MediaAutomation( engine, model, controller, fader,
                                                      defaultVolume, msgDisplay,
                                                      this);
   return automation;
}


