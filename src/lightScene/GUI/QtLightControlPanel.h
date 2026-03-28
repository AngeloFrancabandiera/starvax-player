#ifndef QTPRESETLISTVIEWER_H
#define QTPRESETLISTVIEWER_H

#include "IF_GuiLightControlPanel.h"
#include "QAbstractItemModel"

class ActionListView;
class ActionListController;
class QWidget;
class QListView;
class LightPresetModel;
class LightPresetPlaybar;
class LightPresetDelegate;
class LightsetPainter;
class StatusDisplay;
class ApplicationSettings;
class IF_DmxSliderPanel;



class QtLightControlPanel : public IF_GuiLightControlPanel
{
   Q_OBJECT
public:
   explicit QtLightControlPanel(int numberOfChannels, ActionListController & actionController,
                                 LightPresetModel *model, IF_DmxSliderPanel * dmxPanel,
                                 StatusDisplay & msgDisplay,
                                 const ApplicationSettings &settings,
                                 QWidget *parent);

   ~QtLightControlPanel();

public slots:
   void shiftFocusedUp() override;
   void shiftFocusedDown() override;
   /** remove all selected presets */
   void deleteSelected() override;
   /** enable or disable editing buttons in playbar */
   void setEditMode( bool editMode) override;
   /** de-activate any active preset */
   void deactivate() override;

   void triggerActivePreset() override;

   /**
    * @brief to be called when a channel is set as "manual only"
    * @param isManual
    * @param channel is zero based
    */
   void onManualOnlyChangedByGUI( bool isManual, int channel) override;

private:
   ActionListController & m_actionController;
   ActionListView * m_presetsView;
   LightPresetPlaybar * m_controlBar;
   QAbstractItemModel * m_presetModel;
   IF_DmxSliderPanel * m_dmxPanel;
   LightsetPainter * m_lightsetPainter;
   LightPresetDelegate * m_delegate;

private slots:
   void onPlayRequest();
   void onEditRequest();
   void onActiveChanged(int row);
   void updateControlBar();
   void onUpdateFromSliderRequest();

private:
   void applyLevelsOfIndex(const QModelIndex &rowIndex);
};

#endif // QTPRESETLISTVIEWER_H
