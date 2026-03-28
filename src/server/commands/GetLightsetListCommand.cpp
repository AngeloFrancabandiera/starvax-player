#include "GetLightsetListCommand.h"
#include "LightPresetModel.h"
#include "CommandReply_IF.h"



Server::GetLightsetListCommand::GetLightsetListCommand( LightPresetModel & lightModel,
                                                        CommandReply_IF & replySink) :
   m_lightModel( lightModel),
   m_replySink( replySink)
{
}


bool Server::GetLightsetListCommand::execute(const QStringList & /*parameters*/)
{
   QStringList lightSetList;
   int numberOfPresets = m_lightModel.rowCount();

   for (int i=0; i < numberOfPresets; i++)
   {
      lightSetList << m_lightModel.index(i).data( Qt::DisplayRole).toString();
   }

   m_replySink.sendReplay( Server::COMMAND_ACK, GET_LIGHT_LIST,
                           lightSetList);

   return true;
}

