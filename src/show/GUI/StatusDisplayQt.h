#ifndef STATUSDISPLAYQT_H
#define STATUSDISPLAYQT_H

#include "StatusDisplay.h"
#include <QMap>
#include <QList>
#include <QObject>
#include <QPixmap>

class QStatusBar;
class QLabel;
class QTimer;

/** helper class that calls \a StatusDisplayQt on timer event */
class DisplayListener;

/**
 * @brief The StatusDisplayQt implements status
 * notification on application taskbar
 */
class StatusDisplayQt : public StatusDisplay
{
public:
   StatusDisplayQt( QStatusBar & statusBar);
   ~StatusDisplayQt() override;

   // StatusDisplay interface
public:
   void addIcon(IconType icon) override;
   void removeIcon(IconType icon) override;
   void showMessage(const QString & message, IconType type = NO_ICON) override;
   void showBlockingMessage(const QString & message, IconType type) override;

private:
   /** a message with relevant type */
   struct RichMessage
   {
      QString content;
      IconType type;

      RichMessage( IconType aType, QString aContent) :
         content(aContent),
         type( aType)
      {}
   };

   QStatusBar & m_statusBar;
   QMap<IconType, QPixmap * > m_iconTable;
   QMap<IconType, QLabel * > m_labelTable;

   /** a label used to show mwssages. It seems that QT toolbar
    * hides icons when a message is shown. For this we need a dedicated
    * label with its own timer */
   QLabel * m_messageArea;

   /** this is used to show an icon close to a message to identify message type */
   QLabel * m_messageAreaIcon;

   QList< const RichMessage *> m_messages;
   QTimer * m_timer;
   DisplayListener * m_listener;

private:
   friend class DisplayListener;
   /* this class does not inheirit from QObject */
   void onTimerExpired();
   QPixmap pixmapForType( IconType type);

};

class DisplayListener : public QObject
{
   Q_OBJECT
public:
   DisplayListener( StatusDisplayQt * owner) :
      m_owner( owner)
   {
   }

   ~DisplayListener() {}

public slots:
   void onTimerExpired() {
      m_owner->onTimerExpired();
   }

private:
   StatusDisplayQt * m_owner;
};

#endif // STATUSDISPLAYQT_H
