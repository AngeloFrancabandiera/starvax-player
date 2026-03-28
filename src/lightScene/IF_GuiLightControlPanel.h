#ifndef IF_GUI_LIGHTCONTROLPANEL_H
#define IF_GUI_LIGHTCONTROLPANEL_H

#include <QWidget>
#include <QModelIndex>

class QAbstractItemModel;

/** @class IF_GuiLightControlPanel
 * This interface represents the actions that can be triggered from a GUI
 * to the ligth control functionality. This includes both changing model
 * and activating transitions.
 */

class IF_GuiLightControlPanel : public QWidget
{
   Q_OBJECT
public:
   /**
    * constructor. \p parent is used for parenting and deletion,
    * not for placement in screen
    */
   IF_GuiLightControlPanel( QWidget *parent = nullptr)  : QWidget(parent) { }
   virtual ~IF_GuiLightControlPanel() {}

public slots:
   /** @{
    *  move focused item one position up or down
    */
   virtual void shiftFocusedUp() = 0;
   virtual void shiftFocusedDown() = 0;
   /** @} */

   /** enable or disable editing of presets */
   virtual void setEditMode( bool editMode) = 0;

   /** remove all selected presets */
   virtual void deleteSelected() = 0;

   /** de-activate any active preset */
   virtual void deactivate() = 0;

   /**
    * activate transition that currently holds the playbar
    */
   virtual void triggerActivePreset() = 0;

   /**
    * @brief to be called when a channel is set as "manual only"
    * @param isManual
    * @param channel is zero based
    */
   virtual void onManualOnlyChangedByGUI( bool isManual, int channel) = 0;

signals:
   /**
    * @brief GUI signal that 'play' button has been pressed for a preset
    * @param label - name of preset
    */
   void triggerTransitionRequest( const QString &label);
};

#endif // IF_GUI_LIGHTCONTROLPANEL_H
