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
   QString lineLabel;  /**< label for LINE A or LINE B */
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

/** this table maps an action name to its data for LINE A */
QMap<QString, const ActionDescriptor *> ACTION_TABLE_LINE_A;
/** this table maps an action name to its data for LINE B */
QMap<QString, const ActionDescriptor *> ACTION_TABLE_LINE_B;

QMap<PlaylistFunctionFactory::ActionModifier, const QMap<QString, const ActionDescriptor *> * > ModifierToMapTable;

}  // namespace


PlaylistFunctionFactory::PlaylistFunctionFactory(QObject *parent) :
   QObject(parent)
{
   ACTION_TABLE_LINE_A["play media"] = new ActionDescriptor( IconPath("track_play_A.png"), tr(" Line A"), (Qt::Key_P));
   ACTION_TABLE_LINE_A["pause media"] = new ActionDescriptor(  IconPath("track_pause_A.png"), tr(" Line A"), (Qt::ALT | Qt::Key_P));
   ACTION_TABLE_LINE_A["play/pause media"] = new ActionDescriptor(  "", tr(" Line A"), (Qt::Key_Space));
   ACTION_TABLE_LINE_A["toggle Mute"] = new ActionDescriptor(  "",tr(" Line A"), (Qt::Key_M));
   ACTION_TABLE_LINE_A["rewind"] = new ActionDescriptor( IconPath("track_rewind_A.png"),tr(" Line A"), (Qt::Key_Home));
   ACTION_TABLE_LINE_A["stop"] = new ActionDescriptor( IconPath("track_stop_A.png"),tr(" Line A"), (Qt::Key_A));
   ACTION_TABLE_LINE_A["run/stop fading"] = new ActionDescriptor(  IconPath("fade_out_A.png"), tr(" Line A"), (Qt::Key_F));
   ACTION_TABLE_LINE_A["apply default volume"] = new ActionDescriptor(  IconPath("def_vol_A.png"), tr(" Line A"), (Qt::Key_V));
   ACTION_TABLE_LINE_A["move forward a step"] = new ActionDescriptor(  QString(), tr(" Line A"), (Qt::Key_PageUp));
   ACTION_TABLE_LINE_A["move backward a step"] = new ActionDescriptor(  QString(), tr(" Line A"), (Qt::Key_PageDown));

   ACTION_TABLE_LINE_B["play media"] = new ActionDescriptor(  IconPath("track_play_B.png"), tr(" Line B"), (Qt::SHIFT | Qt::Key_P));
   ACTION_TABLE_LINE_B["pause media"] = new ActionDescriptor(  IconPath("track_pause_B.png"), tr(" Line B"), (Qt::SHIFT | Qt::ALT | Qt::Key_P));
   ACTION_TABLE_LINE_B["play/pause media"] = new ActionDescriptor(  "", tr(" Line B"), (Qt::SHIFT | Qt::Key_Space));
   ACTION_TABLE_LINE_B["toggle Mute"] = new ActionDescriptor(  "",tr(" Line B"), (Qt::SHIFT | Qt::Key_M));
   ACTION_TABLE_LINE_B["rewind"] = new ActionDescriptor(  IconPath("track_rewind_B.png"),tr(" Line B"), (Qt::SHIFT | Qt::Key_Home));
   ACTION_TABLE_LINE_B["stop"] = new ActionDescriptor( IconPath("track_stop_B.png"),tr(" Line B"), (Qt::Key_B));
   ACTION_TABLE_LINE_B["run/stop fading"] = new ActionDescriptor(  IconPath("fade_out_B.png"), tr(" Line B"), (Qt::SHIFT | Qt::Key_F));
   ACTION_TABLE_LINE_B["apply default volume"] = new ActionDescriptor(  IconPath("def_vol_B.png"), tr(" Line B"), (Qt::SHIFT | Qt::Key_V));
   ACTION_TABLE_LINE_B["move forward a step"] = new ActionDescriptor(  QString(), tr(" Line B"), (Qt::SHIFT | Qt::Key_PageUp));
   ACTION_TABLE_LINE_B["move backward a step"] = new ActionDescriptor(  QString(), tr(" Line B"), (Qt::SHIFT | Qt::Key_PageDown));

   ModifierToMapTable[BaseActions] = & ACTION_TABLE_LINE_A;
   ModifierToMapTable[ShiftedActions] = & ACTION_TABLE_LINE_B;
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


QList<QAction *> PlaylistFunctionFactory::buildActionList( IF_MediaEngineInterface *mediaEngine,
                                                           MediaAutomation *automation,
                                                           ActionModifier modifier)
{
   QList<QAction *> actionList;
   const QMap<QString, const ActionDescriptor *> * actionTable = ModifierToMapTable.value( modifier, nullptr);
   T_ASSERT( actionTable != nullptr);


   const ActionDescriptor * actionDescr = actionTable->value("play media", & NullActionDescriptor);
   Action *playAction = new Action( QIcon(actionDescr->iconPath),
                                    tr("play media %1").arg(actionDescr->lineLabel), this);
   playAction->bindShortcut( actionDescr->keySequence);
   playAction->setEnabled( true);
   connect( playAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::play);

   actionDescr = actionTable->value("pause media", & NullActionDescriptor);
   Action *pauseAction = new Action( QIcon(actionDescr->iconPath),
                                     tr("pause media %1").arg( actionDescr->lineLabel), this);
   playAction->setEnabled( true);
   pauseAction->bindShortcut( actionDescr->keySequence);
   connect( pauseAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::pause );

   actionDescr = actionTable->value("play/pause media", & NullActionDescriptor);
   Action *togglePlayAction = new Action( QIcon(actionDescr->iconPath),
                                          tr("play/pause media %1").arg(actionDescr->lineLabel), this);
   togglePlayAction->bindShortcut( actionDescr->keySequence);
   connect( togglePlayAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::togglePlayPause );

   actionDescr = actionTable->value("toggle Mute", & NullActionDescriptor);
   Action *muteAction = new Action( QIcon(actionDescr->iconPath),
                                    tr("toggle Mute %1").arg(actionDescr->lineLabel), this);
   muteAction->bindShortcut( actionDescr->keySequence);
   muteAction->setCheckable( true);
   muteAction->setChecked( false);
   connect( muteAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::setMuted );
   connect( mediaEngine, & IF_MediaEngineInterface::muteStatus, muteAction, & QAction::setChecked );

   actionDescr = actionTable->value("rewind", & NullActionDescriptor);
   Action *rewindAction = new Action( QIcon(actionDescr->iconPath),
                                      tr("rewind %1").arg( actionDescr->lineLabel), this);
   rewindAction->bindShortcut( actionDescr->keySequence);
   connect( rewindAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::rewind );
   rewindAction->setEnabled( true);

   actionDescr = actionTable->value("stop", & NullActionDescriptor);
   Action *stopAction = new Action( QIcon(actionDescr->iconPath),
                                    tr("stop %1").arg(actionDescr->lineLabel), this);
   stopAction->bindShortcut( actionDescr->keySequence);
   connect( stopAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::stop );
   stopAction->setEnabled( true);

   actionDescr = actionTable->value("run/stop fading", & NullActionDescriptor);
   Action *toggleFadeAction = new Action( QIcon(actionDescr->iconPath),
                                          tr("run/stop fading %1").arg(actionDescr->lineLabel), this);
   toggleFadeAction->bindShortcut( actionDescr->keySequence);
   connect( toggleFadeAction, SIGNAL(triggered()), automation, SLOT(toggleFader()) );

   actionDescr = actionTable->value("apply default volume", & NullActionDescriptor);
   Action *defaultVolumeAction = new Action( QIcon(actionDescr->iconPath),
                                             tr("apply default volume %1").arg(actionDescr->lineLabel),
                                             this);
   defaultVolumeAction->bindShortcut( actionDescr->keySequence);
   connect( defaultVolumeAction, & QAction::triggered, automation, & MediaAutomation::applyDefaultVolume );

   actionDescr = actionTable->value("move forward a step", & NullActionDescriptor);
   Action *stepForwardAction = new Action(tr("move forward a step %1").arg(actionDescr->lineLabel), this);
   stepForwardAction->bindShortcut( actionDescr->keySequence);
   connect( stepForwardAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::stepForward );

   actionDescr = actionTable->value("move backward a step", & NullActionDescriptor);
   Action *stepBackwardAction = new Action(tr("move backward a step %1").arg(actionDescr->lineLabel), this);
   stepBackwardAction->bindShortcut( actionDescr->keySequence);
   connect( stepBackwardAction, & QAction::triggered, mediaEngine, & IF_MediaEngineInterface::stepBackward );

   actionList << togglePlayAction << playAction << pauseAction << muteAction
              << rewindAction << stopAction << stepForwardAction << stepBackwardAction
              << toggleFadeAction << defaultVolumeAction;

   MediaToolbar *mediaTools = new MediaToolbar( playAction, pauseAction,
                                                rewindAction, stopAction, this);

   connect( mediaEngine, & IF_MediaEngineInterface::pictureShowChanged,
            mediaTools, & MediaToolbar::onPictureShowChanged );

   connect( mediaEngine, & IF_MediaEngineInterface::currentMediaChanged,
            mediaTools, & MediaToolbar::onMediaChanged );

   return actionList;
}

