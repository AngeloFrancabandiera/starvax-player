#ifndef LIGHTCONTROLENGINE_H
#define LIGHTCONTROLENGINE_H

#include "IF_LightEngineInterface.h"


class LightPresetModel;
class IF_LightTransitionEngine;
class LightPresetData;
class ActionListController;
class DmxInterface;
class StatusDisplay;


class LightEngine : public IF_LightEngineInterface
{
   Q_OBJECT
public:
   explicit LightEngine( LightPresetModel & model,
                         IF_LightTransitionEngine & transitionEngine,
                         ActionListController & viewController,
                         DmxInterface & dmxInterface,
                         StatusDisplay & msgDisplay,
                         QObject *parent = nullptr);

   virtual void requestDmxStart();
   virtual void requestDmxStop();

public slots:
   virtual void setLevelForChannel(double level, int channel);
   virtual void activateByName( const QString &label, bool triggerNow);
   virtual void triggerTransition(const QString &label);
   virtual void freezeTransition();

private:
   LightPresetModel & m_model;
   IF_LightTransitionEngine & m_transitionEngine;
   ActionListController & m_viewController;
   DmxInterface & m_dmxInterface;
   StatusDisplay & m_msgDisplay;

private slots:
   void onTransitionStep(const QList<double> &newValues);
};

#endif // LIGHTCONTROLENGINE_H
