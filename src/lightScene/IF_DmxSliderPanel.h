#ifndef IF_DMXSLIDERPANEL_H
#define IF_DMXSLIDERPANEL_H

#include <QWidget>
#include <QList>

class IF_DmxSliderPanel : public QWidget
{
   Q_OBJECT
public:
   IF_DmxSliderPanel( QWidget *parent = nullptr) : QWidget(parent) { }
   virtual ~IF_DmxSliderPanel() {}

   virtual const QList<double> & values() = 0;

public slots:
   /**
    * @brief application asks GUI to set a list of values for channels
    * @param values - are in range 0.0 - 100.0 (precent of full light)
    */
   virtual void setAllValues( const QList<double> &values) = 0;
   virtual void setValueAtChannel( double value, int channel) = 0;

   /**
    * @brief this slot is used to customize aspect and behaviour when
    *    DMX device is turned on.
    */
   virtual void setDmxOn() = 0;

   /**
    * @brief this slot is used to customize aspect and behaviour when
    *    DMX device is turned off.
    */
   virtual void setDmxOff() = 0;

   /**
    * @brief set a DMX channel as manual only
    * @param manual
    * @param channel
    */
   virtual void setManualOnlyChannel( bool manual, int channel) = 0;

   /**
    * @brief to be called when GUI requests to create preset.
    * Implementation will request to create preset with current values
    */
   virtual void onRequestToCreatePreset() = 0;

signals:
   /**
    * @brief GUI signals that user has changed slider \p channel to value \p value
    * @param value - range 0.0 - 100.0
    * @param channel - 0 based value
    */
   void valueChangedByGUI( double value, int channel );

   /**
    * @brief GUI signals that user has changed "manual only" for a given channel
    * @param isManual is TRUE for "manual only"
    * @param channel - 0 based value
    */
   void manualOnlyChanged( bool isManual, int channel);

   /**
    * @brief emitted when user has requested to create a preset with
    *    current values of channels sliders
    * @param values
    */
   void createPresetFromValues( const QList<double> &values);
};

#endif // IF_DMXSLIDERPANEL_H
