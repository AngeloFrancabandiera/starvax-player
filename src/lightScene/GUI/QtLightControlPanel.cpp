#include "QtLightControlPanel.h"
#include "qwidget.h"
#include "testableAssert.h"
#include "QListView"
#include "QVBoxLayout"
#include "IF_DmxSliderPanel.h"
#include "QAbstractItemModel"
#include "LightPresetDelegate.h"
#include "LightPresetData.h"
#include "LightsetPainter.h"
#include "LightPresetPlaybar.h"
#include "LightPresetModel.h"
#include "LightsetListView.h"
#include "modelViewRules.h"
#include "ActionListController.h"
#include "ActionListView.h"
#include "StatusDisplay.h"
#include "ApplicationSettings.h"


/** @class QtLightControlPanel
 * implementation of ::IF_GuiLightControlPanel interface that uses Qt widgets.
 * Items are displayed in a list view with activation controller, (see ::ActionListView).
 */

#define CONTROL_BAR_MIN_HEIGHT     50

/* GUI controls are generated outside factory */

QtLightControlPanel::QtLightControlPanel( int numberOfChannels,
                                          ActionListController &actionController,
                                          LightPresetModel *model,
                                          IF_DmxSliderPanel * dmxPanel,
                                          StatusDisplay & msgDisplay,
                                          const ApplicationSettings & settings,
                                          QWidget *parent) :
   IF_GuiLightControlPanel(parent),
   m_actionController(actionController),
   m_dmxPanel( dmxPanel)
{
   m_presetModel = model;
   m_lightsetPainter = new LightsetPainter( settings);
   m_presetsView = new LightsetListView( actionController, m_lightsetPainter,
                                         msgDisplay, this);
   m_presetsView->setModel( model);
   m_presetsView->setTitle( tr("Light control"));
   m_presetsView->setObjectName( "LightView");

   m_presetsView->setDragEnabled( true);
   m_presetsView->setAcceptDrops( true);

   QHBoxLayout *layout = new QHBoxLayout( this);
   setLayout( layout );
   layout->setContentsMargins( 2,2,2,2);
   layout->addWidget( m_presetsView);

   m_presetsView->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);

   m_delegate = new LightPresetDelegate( numberOfChannels, this);
   m_presetsView->setItemDelegate( m_delegate );

   m_controlBar = new LightPresetPlaybar( m_presetsView);
   m_controlBar->setMinimumHeight( CONTROL_BAR_MIN_HEIGHT );
   m_controlBar->setVisible( false);

   m_presetsView->setActionBar( m_controlBar);

   connect( m_controlBar, & LightPresetPlaybar::play, this, & QtLightControlPanel::onPlayRequest );
   connect( m_controlBar, & LightPresetPlaybar::edit, this, & QtLightControlPanel::onEditRequest );
   connect( m_controlBar, & LightPresetPlaybar::remove,
            m_presetsView, & ActionListView::deleteActiveItem );
   connect( m_controlBar, & LightPresetPlaybar::updateFromSliders,
            this, & QtLightControlPanel::onUpdateFromSliderRequest);

   connect( &m_actionController, & ActionListController::activeRowChanged,
            this, & QtLightControlPanel::onActiveChanged);

   /* when model change, control bar must also be updated */
   connect( m_presetModel, SIGNAL(layoutChanged()),
            this, SLOT(updateControlBar()) );

   connect( & settings, SIGNAL(panelFontSizeChanged(int)),
            m_presetsView, SLOT(onPanelFontSizeChanged(int)) );

   connect( m_presetsView, &ActionListView::redoRequest, model, &LightPresetModel::redo);
   connect( m_presetsView, &ActionListView::undoRequest, model, &LightPresetModel::undo);
}

QtLightControlPanel::~QtLightControlPanel()
{
   delete m_presetsView;
   delete m_lightsetPainter;
}

/**
 * @brief callback when GUI request to activate preset.
 */
void QtLightControlPanel::onPlayRequest()
{
   T_ASSERT(m_presetModel != nullptr);

   try
   {
      int activeRow = m_actionController.activeItemRow();
      const LightPresetData & preset =
            m_presetModel->index(activeRow, 0).data(modelViewRules::Binary).value<LightPresetData>();

      QString activePresetLabel = preset.name();
      emit triggerTransitionRequest( activePresetLabel);
   }
   catch (QString &)
   {
      T_ASSERT(false);
   }
}

/**
 * @brief callback when GUI requests to edit the preset.
 */
void QtLightControlPanel::onEditRequest()
{
   int activeRow = m_actionController.activeItemRow();
   m_presetsView->edit( m_presetModel->index( activeRow, 0) );
}

void QtLightControlPanel::onActiveChanged(int  row)
{
   QModelIndex rowIndex = m_presetModel->index( row, 0);

   applyLevelsOfIndex( rowIndex);
}

/** update current light values to control bar */
void QtLightControlPanel::updateControlBar()
{
   QModelIndex rowIndex = m_presetModel->index( m_actionController.activeItemRow(), 0);
   applyLevelsOfIndex(rowIndex);
}

void QtLightControlPanel::onUpdateFromSliderRequest()
{
   /* get current index and update it */
   int activeRow = m_actionController.activeItemRow();
   T_ASSERT( activeRow >= 0);

   const LightPresetData & oldPreset =
         m_presetModel->data( m_presetModel->index(activeRow, 0),
                             modelViewRules::Binary).value<LightPresetData >();
   LightPresetData newPreset = oldPreset;

   newPreset.setLevelList( m_dmxPanel->values());

   m_presetModel->setData( m_presetModel->index(activeRow, 0),
                           QVariant::fromValue<LightPresetData>(newPreset), modelViewRules::Binary);
}


void QtLightControlPanel::applyLevelsOfIndex(const QModelIndex & rowIndex)
{
   if (rowIndex != QModelIndex())
   {
      const LightPresetData & preset =
            rowIndex.data( modelViewRules::Binary).value<LightPresetData >();

      m_controlBar->show();
      m_controlBar->setValueList( preset.levelList());
   }
   else
   {
      m_controlBar->hide();
   }
}

void QtLightControlPanel::shiftFocusedUp()
{
   m_presetsView->shiftFocusedUp();
}

void QtLightControlPanel::shiftFocusedDown()
{
   m_presetsView->shiftFocusedDown();
}

void QtLightControlPanel::deleteSelected()
{
   m_presetsView->requestToDeleteSelection();
}

void QtLightControlPanel::setEditMode(bool editMode)
{
   m_controlBar->setEditMode( editMode);
   m_presetsView->setEditMode( editMode);

   if (editMode)
   {
      /* allow to edit with F2, but not with double click because
       * we want to activate a preset also in edit mode */
      m_presetsView->setEditTriggers( QAbstractItemView::EditKeyPressed);
   }
   else
   {
      m_presetsView->setEditTriggers( QAbstractItemView::NoEditTriggers);
   }
}

void QtLightControlPanel::deactivate()
{
   m_presetsView->deActivate();
}

void QtLightControlPanel::triggerActivePreset()
{
   onPlayRequest();
}

void QtLightControlPanel::onManualOnlyChangedByGUI( bool isManual, int channel)
{
   m_controlBar->setManualOnly( isManual, channel);
}
