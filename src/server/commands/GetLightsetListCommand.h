#ifndef GETLIGHTSETLISTCOMMAND_H
#define GETLIGHTSETLISTCOMMAND_H

#include "Command_IF.h"
class LightPresetModel;



namespace Server {

class CommandReply_IF;

/**
 * @brief The GetLightsetListCommand returns all light presets
 *   in current show.
 */
class GetLightsetListCommand : public Command_IF
{
public:
   GetLightsetListCommand( LightPresetModel & lightModel,
                           CommandReply_IF & replySink);
   ~GetLightsetListCommand() override {}

   // Command_IF interface
public:
   quint8 operativeCode() const override {
      return GET_LIGHT_LIST;
   }

   bool execute(const QStringList & parameters) override;

private:
   LightPresetModel & m_lightModel;
   CommandReply_IF & m_replySink;
};

}  // namespace Server

#endif // GETLIGHTSETLISTCOMMAND_H
