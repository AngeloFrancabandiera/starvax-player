#ifndef SELECTLIGHTTRANSITIONCOMMAD_H
#define SELECTLIGHTTRANSITIONCOMMAD_H

#include "Command_IF.h"
class IF_LightEngineInterface;
class LightPresetModel;


namespace Server {

class CommandReply_IF;


/**
 * @brief The SelectLightTransitionCommand sets a light
 *   preset as active.
 * Two parametrs are needed: the first is preset name; the second must
 * be either "START" or "DONT_START", according to which the preset
 * is immediately fired
 */
class SelectLightTransitionCommand : public Command_IF
{
public:
   SelectLightTransitionCommand( IF_LightEngineInterface & lightEngine,
                                 LightPresetModel & lightModel,
                                 CommandReply_IF & replySink);

   ~SelectLightTransitionCommand() override {}

   // Command_IF interface
public:
   quint8 operativeCode() const override{
      return CONTROL_LIGHT_PRESET;
   }

   bool execute(const QStringList & parameters) override;

private:
   IF_LightEngineInterface & m_lightEngine;
   LightPresetModel & m_lightModel;
   CommandReply_IF & m_replySink;
   bool executeCommand( const QString & startFlag, const QString & presetName);
};

}  // namespace Server

#endif // SELECTLIGHTTRANSITIONCOMMAD_H
