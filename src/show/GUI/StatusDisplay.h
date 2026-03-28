#ifndef STATUSDISPLAY_H
#define STATUSDISPLAY_H

class QString;

class StatusDisplay
{
public:
   StatusDisplay(){}
   virtual ~StatusDisplay() {}

   /** icons that can be shown in notification area */
   enum IconType
   {
      NO_ICON = 0,
      ALL,            /**< only for remove */
      SERVER_ACTIVE,
      SERVER_CONNECTING,
      SERVER_CONNECTED,
      WARNING,
      INFORMATION
   };

   /**
    * @brief show an icon in notification area
    * @param icon
    */
   virtual void addIcon( IconType icon) = 0;

   /**
    * @brief remove an icon from notification area, if present
    * @param icon
    */
   virtual void removeIcon( IconType icon) = 0;

   /**
    * @brief show a message in notification area. According
    *   to implemetation, the massage may be shown for a limited
    *   amount of time.
    * @param message
    */
   virtual void showMessage( const QString & message, IconType type = NO_ICON) = 0;

   /**
    * @brief show a message that must check user attention. In GUI, this is a message
    *   box instead of a message in notification area.
    * @param message
    * @param type
    */
   virtual void showBlockingMessage( const QString & message, IconType type = NO_ICON) = 0;
};

#endif // STATUSDISPLAY_H
