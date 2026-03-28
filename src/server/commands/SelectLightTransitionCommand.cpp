#include "SelectLightTransitionCommand.h"
#include <QStringList>

#include "IF_LightEngineInterface.h"
#include "LightPresetModel.h"
#include "CommandReply_IF.h"



Server::SelectLightTransitionCommand::
SelectLightTransitionCommand( IF_LightEngineInterface & lightEngine,
                              LightPresetModel & lightModel,
                              CommandReply_IF & replySink) :
   m_lightEngine( lightEngine),
   m_lightModel( lightModel),
   m_replySink( replySink)
{
}


bool Server::SelectLightTransitionCommand::executeCommand( const QString & startFlag,
                                                           const QString & presetName)
{
   bool commandIsLegal = false;

   if (startFlag == "START")
   {
      m_lightEngine.activateByName( presetName,
                                    IF_LightEngineInterface::START_NOW);

      commandIsLegal = true;
      m_replySink.sendReplay( Server::COMMAND_ACK, CONTROL_LIGHT_PRESET, "all fine");
   }
   else if (startFlag == "DONT_START")
   {
      m_lightEngine.activateByName( presetName,
                                    IF_LightEngineInterface::DONT_START_NOW);
      commandIsLegal = true;
      m_replySink.sendReplay( Server::COMMAND_ACK, CONTROL_LIGHT_PRESET, "done");
   }
   else
   {
      m_replySink.sendReplay( Server::COMMAND_INVALID_ARGUMENT, CONTROL_LIGHT_PRESET,
                              QString("Second argument must be either "
                                      "START or DONT_START "));
   }

   return commandIsLegal;
}

bool Server::SelectLightTransitionCommand::execute( const QStringList & parameters)
{
   QString presetName;
   bool commandIsLegal = false;

   if (parameters.size() >= 2)
   {
      presetName = parameters.first();

      if (m_lightModel.searchByName( presetName) != QModelIndex())
      {
         QString startFlag = parameters.at(1);

         commandIsLegal = executeCommand(startFlag, presetName);
      }
      else
      {
         m_replySink.sendReplay( Server::COMMAND_LIGHTSET_NOT_FOUND, CONTROL_LIGHT_PRESET,
                                 QString("Not found: %1").arg(presetName));
      }
   }
   else
   {
      m_replySink.sendReplay( Server::COMMAND_NOT_ENOUGH_ARGS, CONTROL_LIGHT_PRESET,
                              QString("Two arguments are required: "
                                      "preset name and 'start immediate' flag"));
   }

   return commandIsLegal;
}

