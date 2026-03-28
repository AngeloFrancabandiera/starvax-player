#include "ServerStatus.h"
#include "StatusDisplay.h"
#include <QAction>

Server::Status::Status( QAction & startAction,
                        QAction & stopAction,
                        StatusDisplay & statusDisplay) :
   m_startAction( startAction),
   m_stopAction( stopAction),
   m_statusDisplay( statusDisplay)
{
   m_startAction.setEnabled( true);
   m_stopAction.setEnabled( false);
}

void Server::Status::onServerStarted()
{
   m_statusDisplay.addIcon( StatusDisplay::SERVER_ACTIVE);

   m_startAction.setEnabled( false);
   m_stopAction.setEnabled( true);

   m_statusDisplay.showMessage( QString("<b>SERVER:</b> started"), StatusDisplay::INFORMATION);
}

void Server::Status::onServerStopped()
{
   m_statusDisplay.removeIcon( StatusDisplay::ALL);

   m_startAction.setEnabled( true);
   m_stopAction.setEnabled( false);

   m_statusDisplay.showMessage( QString("<b>SERVER:</b> stopped"), StatusDisplay::INFORMATION);
}

void Server::Status::onConnecting()
{
   m_statusDisplay.removeIcon( StatusDisplay::SERVER_CONNECTED);
   m_statusDisplay.addIcon( StatusDisplay::SERVER_CONNECTING);

   m_statusDisplay.showMessage( QString("<b>SERVER:</b> connecting"), StatusDisplay::INFORMATION);
}

void Server::Status::onConnectionUp()
{
   m_statusDisplay.removeIcon( StatusDisplay::SERVER_CONNECTING);
   m_statusDisplay.addIcon( StatusDisplay::SERVER_CONNECTED);

   m_statusDisplay.showMessage( QString("<b>SERVER:</b> connection established"));
}

void Server::Status::onConnectionClosed()
{
   m_statusDisplay.removeIcon( StatusDisplay::SERVER_CONNECTING);
   m_statusDisplay.removeIcon( StatusDisplay::SERVER_CONNECTED);

   m_statusDisplay.showMessage( QString("<b>SERVER:</b> connection closed"), StatusDisplay::INFORMATION);
}

void Server::Status::onError(const QString & error)
{
   m_statusDisplay.showMessage( QString("<b>SERVER: ERR: </b> ") + error, StatusDisplay::INFORMATION);
}
