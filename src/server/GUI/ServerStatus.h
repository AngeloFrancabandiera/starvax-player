#ifndef SERVERSTATUS_H
#define SERVERSTATUS_H

class QAction;
class QString;
class StatusDisplay;

namespace Server {


class Status
{
public:
   /** constructor. All objects must have application
    * lifetime */
   Status( QAction & startAction,
           QAction & stopAction,
           StatusDisplay & statusDisplay);

   ~Status() {}

   /**
    * @brief onServerStarted
    */
   void onServerStarted();

   /**
    * @brief onServerStopped
    */
   void onServerStopped();

   /**
    * @brief onConnecting
    */
   void onConnecting();

   /**
    * @brief onConnectionUp
    */
   void onConnectionUp();

   /**
    * @brief onConnectionUp
    */
   void onConnectionClosed();

   /**
    * @brief onError
    * @param error
    */
   void onError( const QString & error);

private:
   QAction & m_startAction;
   QAction & m_stopAction;
   StatusDisplay & m_statusDisplay;
};

}  // namespace Server

#endif // SERVERSTATUS_H
