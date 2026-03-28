#ifndef LIGHTCONTROLGUIFACTORY_H
#define LIGHTCONTROLGUIFACTORY_H

#include <QObject>

class QWidget;
class QLayout;

class IF_DmxSliderPanel;
class IF_GuiLightControlPanel;
class IF_LightEngineInterface;
class IF_LightTransitionEngine;
class LightPresetModel;
class ActionListController;
class QAction;
class ApplicationSettings;
class StatusDisplay;


class LightControlGuiFactory : public QObject
{
   Q_OBJECT
public:
   explicit LightControlGuiFactory(int numOfCahnnels, const ApplicationSettings & settings,
                                   QWidget *parent = nullptr);

   IF_DmxSliderPanel *buildDmxSliderPanel( IF_LightEngineInterface *engine,
                                           IF_LightTransitionEngine *transitionEngine,
                                           ApplicationSettings * applicationSettings,
                                           QWidget *parent);
   IF_GuiLightControlPanel *buildPresetListViewer( IF_LightEngineInterface *engine,
                                                   LightPresetModel *model,
                                                   IF_DmxSliderPanel * dmxPanel,
                                                   ActionListController *actionController,
                                                   StatusDisplay * msgDisplay, QWidget *parent);
   /**
    * build a complete light control panel, assembling the given \p sliderPanel
    * and \p presetView with a toolbar. All widgets are fit into given \p container.
    * Toolbar actions are linked to the given \p engine
    */
   void buildLightControlPanel( IF_LightEngineInterface *engine, LightPresetModel * model,
                                IF_DmxSliderPanel *sliderPanel, IF_GuiLightControlPanel *presetView,
                                QAction * editModeAction, QLayout *container);

   QList<QAction *> buildActions( IF_GuiLightControlPanel *presetView);

private:
   int m_numOfCahnnels;
   const ApplicationSettings & m_settings;

private:
   QWidget *buildPresetToolbar( IF_DmxSliderPanel * sliderPanel,
                                IF_LightEngineInterface *engine,
                                LightPresetModel * model,
                                IF_GuiLightControlPanel *presetView,
                                QAction * editModeAction, QWidget *parent);
};

#endif // LIGHTCONTROLGUIFACTORY_H
