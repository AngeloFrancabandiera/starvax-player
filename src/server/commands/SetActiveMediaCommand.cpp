#include "SetActiveMediaCommand.h"
#include <QStringList>

#include "MediaAutomation.h"
#include "MediaListModel.h"
#include "CommandReply_IF.h"


Server::SetActiveTrackCommand::
SetActiveTrackCommand( MediaAutomation & audioVideoAutomation_A,
                       MediaAutomation & audioVideoAutomation_B,
                       MediaListModel & mediaModel_A,
                       MediaListModel & mediaModel_B,
                       CommandReply_IF & replySink) :
   m_audioVideoAutomation_A( audioVideoAutomation_A),
   m_audioVideoAutomation_B( audioVideoAutomation_B),
   m_mediaModel_A( mediaModel_A),
   m_mediaModel_B( mediaModel_B),
   m_replySink( replySink)
{
}


bool Server::SetActiveTrackCommand::execute(const QStringList & parameters)
{
   QString label;
   QString line;
   m_errorString = QString();
   bool ok = false;

   if (parameters.size() >= 2)
   {
      line = parameters.at(0);
      label = parameters.at(1);

      /* try to activate */
      ok = activateLabelForLine( label, line);

      if (ok)
      {
         m_replySink.sendReplay( Server::COMMAND_ACK, SET_ACTIVE_MEDIA,
                                 QStringList() << "done");
      }
      else
      {
         m_replySink.sendReplay( Server::COMMAND_MEDIA_NOT_FOUND, SET_ACTIVE_MEDIA,
                                 QStringList() << m_errorString);
      }
   }
   else
   {
      m_replySink.sendReplay( Server::COMMAND_NOT_ENOUGH_ARGS, SET_ACTIVE_MEDIA,
                              QStringList() << "Two arguments required: "
                              << "line ('A' or 'B') and track label");
   }

   return ok;
}

bool Server::SetActiveTrackCommand::activateLabelForLine( const QString & label,
                                                          const QString & line)
{
   bool valid = false;
   MediaAutomation * automation = nullptr;
   MediaListModel * model = nullptr;
   QString lineTag;

   if (line == "A")
   {
      automation = & m_audioVideoAutomation_A;
      model = & m_mediaModel_A;
      lineTag = "A";
   }
   else if (line == "B")
   {
      automation = & m_audioVideoAutomation_B;
      model = & m_mediaModel_B;
      lineTag = "B";
   }
   else
   {
      m_errorString = "line must be 'A' or 'B'";
   }

   if (model)
   {
      /* line is valid. Search for label in selected line */
      if (model->searchById( label) != QModelIndex())
      {
         automation->activateMediaById( label, false);
         valid = true;
      }
      else
      {
         m_errorString = QString("can't find %1 in line %2").arg(label).arg(lineTag);
      }
   }

   return valid;
}
