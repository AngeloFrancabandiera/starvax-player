#include <QVBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QSpacerItem>
#include <QCommonStyle>

#include "Action.h"
#include "testableAssert.h"
#include "lightControlGuiFactory.h"
#include "QtDmxSliderPanel.h"
#include "QtLightControlPanel.h"
#include "IF_LightEngineInterface.h"
#include "IF_lighttransitionengine.h"
#include "LightPresetModel.h"
#include "ApplicationIcon.h"
#include "ApplicationSettings.h"


/** @class LightControlGuiFactory
 *
 * concrete factory that implements preset list view with
 * Qt Widgets
 */

LightControlGuiFactory::LightControlGuiFactory(int numOfCahnnels,
                                               const ApplicationSettings &settings,
                                               QWidget *parent) :
   QObject(parent),
   m_numOfCahnnels(numOfCahnnels),
   m_settings( settings)
{
}

IF_DmxSliderPanel *LightControlGuiFactory::buildDmxSliderPanel(IF_LightEngineInterface *engine,
                                                               IF_LightTransitionEngine *transitionEngine,
                                                               ApplicationSettings *applicationSettings,
                                                               QWidget *parent)
{
   QtDmxSliderPanel *panel = new QtDmxSliderPanel( m_numOfCahnnels, transitionEngine, parent);
   connect( panel, SIGNAL(valueChangedByGUI(double,int)),
            engine, SLOT(setLevelForChannel(double,int)) );
   connect( engine, SIGNAL(dmxStarted()), panel, SLOT(setDmxOn()) );
   connect( engine, SIGNAL(dmxStopped()), panel, SLOT(setDmxOff()) );

   connect( panel, SIGNAL(manualOnlyChanged(bool,int)),
            transitionEngine, SLOT(onManualOnlyChanged(bool,int)) );
   connect( panel, SIGNAL(manualOnlyChanged(bool,int)),
            applicationSettings, SLOT(manualOnlyChannelChanged(bool,int)) );

   QList<int> manualOnlyChannels = applicationSettings->manualOnlyChannels();

   foreach (int channel, manualOnlyChannels)
   {
      panel->setManualOnlyChannel( true, channel);
   }

   return panel;
}

IF_GuiLightControlPanel *LightControlGuiFactory::buildPresetListViewer( IF_LightEngineInterface *engine,
                                                                        LightPresetModel *model,
                                                                        IF_DmxSliderPanel * dmxPanel,
                                                                        ActionListController *actionController,
                                                                        StatusDisplay * msgDisplay,
                                                                        QWidget *parent)
{
   T_ASSERT( actionController);
   T_ASSERT( msgDisplay);

   QtLightControlPanel *viewer = new QtLightControlPanel( m_numOfCahnnels, *actionController,
                                                          model, dmxPanel, *msgDisplay, m_settings, parent);

   connect( viewer, SIGNAL(triggerTransitionRequest(QString)),
            engine, SLOT(triggerTransition(QString)) );

   return viewer;
}

void LightControlGuiFactory::buildLightControlPanel( IF_LightEngineInterface *engine,
                                                     LightPresetModel * model,
                                                     IF_DmxSliderPanel *sliderPanel,
                                                     IF_GuiLightControlPanel *presetView,
                                                     QAction * editModeAction,
                                                     QLayout *container)
{
   QSplitter *splitter = new QSplitter( Qt::Vertical, container->parentWidget());
   /* highlight handle */
   splitter->setBackgroundRole( QPalette::Dark );

   QWidget *presetWithToolbar = new QWidget( container->parentWidget());
   presetWithToolbar->setLayout( new QVBoxLayout());
   presetWithToolbar->layout()->setContentsMargins( 2,2,2,2);

   QWidget *toolbar = buildPresetToolbar( sliderPanel, engine, model, presetView,
                                          editModeAction, container->parentWidget());
   presetWithToolbar->layout()->addWidget( toolbar );
   presetWithToolbar->layout()->addWidget( presetView);
   splitter->addWidget( sliderPanel);
   splitter->addWidget( presetWithToolbar);

   container->addWidget(splitter);
   container->setContentsMargins( 2,2,2,2);
}

QList<QAction *> LightControlGuiFactory::buildActions(IF_GuiLightControlPanel *presetView)
{
   T_ASSERT( presetView != nullptr);

   Action *triggerTransitionAction = new Action( tr("trigger active transition"), this);
   triggerTransitionAction->bindShortcut( Qt::Key_F1);

   connect( triggerTransitionAction, & QAction::triggered,
            presetView, & IF_GuiLightControlPanel::triggerActivePreset);

   return QList<QAction *>() << triggerTransitionAction;
}

QWidget *LightControlGuiFactory::buildPresetToolbar( IF_DmxSliderPanel * sliderPanel,
                                                     IF_LightEngineInterface *engine,
                                                     LightPresetModel * model,
                                                     IF_GuiLightControlPanel *presetView,
                                                     QAction * editModeAction,
                                                     QWidget *parent)
{
   QCommonStyle style;
   QSize iconSize = QSize( m_settings.getIconSize(), m_settings.getIconSize());

   QWidget *toolbar = new QWidget( parent);
   toolbar->setLayout( new QHBoxLayout());
   toolbar->layout()->setContentsMargins( 2,2,2,2);

   /* 'DMX on-off' buttons */
   QPushButton *dmxOnButton = new QPushButton( QIcon(IconPath("light.png")), QString(), toolbar);
   dmxOnButton->setToolTip( tr("turn DMX device on"));
   dmxOnButton->setFocusPolicy(Qt::NoFocus);
   connect( dmxOnButton, & QPushButton::clicked, engine, & IF_LightEngineInterface::requestDmxStart);
   toolbar->layout()->addWidget( dmxOnButton);
   dmxOnButton->setMaximumSize(iconSize);

   QPushButton *dmxOffButton = new QPushButton( QIcon(IconPath("no-light.png")), QString(), toolbar);
   dmxOffButton->setToolTip( tr("turn DMX device off"));
   dmxOffButton->setEnabled( false);
   dmxOffButton->setFocusPolicy(Qt::NoFocus);
   connect( dmxOffButton, & QPushButton::clicked, engine, & IF_LightEngineInterface::requestDmxStop);
   toolbar->layout()->addWidget( dmxOffButton);
   dmxOffButton->setMaximumSize(iconSize);

   connect( engine, SIGNAL(dmxStartChanged(bool)), dmxOnButton, SLOT(setDisabled(bool)) );
   connect( engine, SIGNAL(dmxStartChanged(bool)), dmxOffButton, SLOT(setEnabled(bool)) );

   QPushButton *deactivateButton = new QPushButton( QIcon(IconPath("close.png")),
                                                    QString(), toolbar);
   deactivateButton->setToolTip(tr("activate nothing"));
   connect( deactivateButton, & QPushButton::clicked, presetView, & IF_GuiLightControlPanel::deactivate);
   toolbar->layout()->addWidget( deactivateButton);
   deactivateButton->setMaximumSize(iconSize);

   /* spacer */
   toolbar->layout()->addItem( new QSpacerItem( 1,1, QSizePolicy::Expanding));

   /* 'create preset' button */
   QPushButton *createButton = new QPushButton( QIcon(IconPath("add.png")),
                                                QString(), toolbar);
   createButton->setFocusPolicy(Qt::NoFocus);
   connect( createButton, & QPushButton::clicked, sliderPanel, & IF_DmxSliderPanel::onRequestToCreatePreset);
   toolbar->layout()->addWidget( createButton);
   createButton->setToolTip( tr("create a new preset with current values"));
   createButton->setMaximumSize(iconSize);

   /* 'shift up' button */
   QPushButton *shiftUpButton = new QPushButton( style.standardIcon(QStyle::SP_ArrowUp),
                                                 QString(), toolbar);
   shiftUpButton->setToolTip(tr("bring focused element one position above"));
   shiftUpButton->setFocusPolicy(Qt::NoFocus);
   connect( shiftUpButton, & QPushButton::clicked, presetView, & IF_GuiLightControlPanel::shiftFocusedUp);
   toolbar->layout()->addWidget( shiftUpButton);
   shiftUpButton->setMaximumSize(iconSize);

   /* 'shift down' button */
   QPushButton *shiftDownButton = new QPushButton( style.standardIcon(QStyle::SP_ArrowDown),
                                                   QString(), toolbar);
   shiftDownButton->setToolTip(tr("bring focused element one position below"));
   shiftDownButton->setFocusPolicy(Qt::NoFocus);
   connect( shiftDownButton, & QPushButton::clicked, presetView, & IF_GuiLightControlPanel::shiftFocusedDown );
   toolbar->layout()->addWidget( shiftDownButton);
   shiftDownButton->setMaximumSize(iconSize);

   /* 'delete selection' button */
   QPushButton *deleteButton = new QPushButton( style.standardIcon(QStyle::SP_TrashIcon),
                                                   QString(), toolbar);
   deleteButton->setToolTip( tr("delete selected items"));
   deleteButton->setFocusPolicy(Qt::NoFocus);
   connect( deleteButton, SIGNAL(clicked()), presetView, SLOT(deleteSelected()) );
   toolbar->layout()->addWidget( deleteButton);
   deleteButton->setMaximumSize(iconSize);

   /* undo/redo buttons */
   QPushButton *undoButton = new QPushButton( QIcon(IconPath("undo.png")), QString(), toolbar);
   undoButton->setToolTip( tr("undo"));
   undoButton->setFocusPolicy(Qt::NoFocus);
   undoButton->setEnabled( false);
   connect( undoButton, & QPushButton::clicked, model, & LightPresetModel::undo );
   connect( model, & LightPresetModel::canUndoChanged, undoButton, & QPushButton::setEnabled);
   toolbar->layout()->addWidget( undoButton);
   undoButton->setMaximumSize(iconSize);

   QPushButton *redoButton = new QPushButton( QIcon(IconPath("redo.png")), QString(), toolbar);
   redoButton->setToolTip( tr("redo"));
   redoButton->setFocusPolicy(Qt::NoFocus);
   redoButton->setEnabled( false);
   connect( redoButton, & QPushButton::clicked, model, & LightPresetModel::redo );
   connect( model, & LightPresetModel::canRedoChanged, redoButton, & QPushButton::setEnabled);
   toolbar->layout()->addWidget( redoButton);
   redoButton->setMaximumSize(iconSize);

   connect( editModeAction, & QAction::triggered, createButton, & QPushButton::setVisible );
   connect( editModeAction, & QAction::triggered, shiftUpButton, & QPushButton::setVisible );
   connect( editModeAction, & QAction::triggered, shiftDownButton,  & QPushButton::setVisible );
   connect( editModeAction, & QAction::triggered, deleteButton,  & QPushButton::setVisible );
   connect( editModeAction, & QAction::triggered, undoButton,  & QPushButton::setVisible );
   connect( editModeAction, & QAction::triggered, redoButton,  & QPushButton::setVisible );

   connect( editModeAction, & QAction::triggered, presetView, & IF_GuiLightControlPanel::setEditMode);

   return toolbar;
}

