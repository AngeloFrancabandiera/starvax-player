#include "StatusDisplayQt.h"
#include <QStatusBar>
#include <QLabel>
#include <QMessageBox>
#include <QPicture>
#include <QIcon>
#include <QLayout>
#include <QTimer>
#include <QCommonStyle>

#include "ApplicationIcon.h"
#include "testableAssert.h"

namespace  {
const int SHOW_MESSAGE_DURATION_MS = 3000;
}  // namespace



StatusDisplayQt::StatusDisplayQt(QStatusBar & statusBar) :
   m_statusBar( statusBar),
   m_timer( new QTimer()),
   m_listener( new DisplayListener(this))
{
   m_iconTable[SERVER_ACTIVE] = new QPixmap(IconPath("ServerActive.png"));
   m_iconTable[SERVER_CONNECTING] = new QPixmap(IconPath("connecting.png"));
   m_iconTable[SERVER_CONNECTED] = new QPixmap(IconPath("connected.png"));

   QCommonStyle style;
   m_iconTable[WARNING] =
         new QPixmap( style.standardIcon(QStyle::SP_MessageBoxWarning).pixmap(16,16));
   m_iconTable[INFORMATION] =
         new QPixmap( style.standardIcon(QStyle::SP_MessageBoxInformation).pixmap(16,16));

   m_labelTable[SERVER_ACTIVE] = new QLabel("");
   m_labelTable[SERVER_CONNECTING] = new QLabel("");
   m_labelTable[SERVER_CONNECTED] = new QLabel("");

   m_labelTable[SERVER_ACTIVE]->setPixmap( *m_iconTable[SERVER_ACTIVE]);
   m_labelTable[SERVER_CONNECTING]->setPixmap( *m_iconTable[SERVER_CONNECTING]);
   m_labelTable[SERVER_CONNECTED]->setPixmap( *m_iconTable[SERVER_CONNECTED]);

   m_messageArea = new QLabel( & m_statusBar);
   m_messageArea->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred);
   m_messageAreaIcon = new QLabel( & m_statusBar);
   m_statusBar.addWidget( m_messageAreaIcon);
   m_statusBar.addWidget( m_messageArea);
   m_statusBar.addPermanentWidget( m_labelTable[SERVER_ACTIVE]);
   m_statusBar.addPermanentWidget( m_labelTable[SERVER_CONNECTING]);
   m_statusBar.addPermanentWidget( m_labelTable[SERVER_CONNECTED]);

   foreach( QLabel * label, m_labelTable)
   {
      label->setVisible( false);
      label->setMaximumSize( 32, 32);
      m_statusBar.layout()->setAlignment( label, Qt::AlignRight);
   }

   QObject::connect( m_timer, SIGNAL(timeout()), m_listener, SLOT(onTimerExpired()));
}

StatusDisplayQt::~StatusDisplayQt()
{
   m_iconTable.clear();
   m_labelTable.clear();

   m_timer->stop();
   delete m_timer;
   delete m_listener;
}


void StatusDisplayQt::onTimerExpired()
{
   T_ASSERT( m_timer->isActive());

   if ( ! m_messages.isEmpty())
   {
      /* show next message */
      const RichMessage * message = m_messages.takeFirst(); // this de-allocated memory for item
      T_ASSERT( message);
      m_messageArea->setText( message->content);
      m_messageAreaIcon->setPixmap( pixmapForType(message->type));
   }
   else
   {
      /* nothing else to show */
      m_messageArea->setText("");
      m_messageAreaIcon->setPixmap( QPixmap());
      m_timer->stop();
   }
}

void StatusDisplayQt::addIcon(IconType icon)
{
   /* icon 'ALL' is not supported */
   T_ASSERT( m_labelTable[icon]);
   m_labelTable[icon]->setVisible( true);
}

void StatusDisplayQt::removeIcon(IconType icon)
{
   if (icon == StatusDisplay::ALL)
   {
      foreach( QLabel * label, m_labelTable)
      {
         T_ASSERT( label);
         label->setVisible( false);
      }
   }
   else
   {
      T_ASSERT( m_labelTable[icon]);
      m_labelTable[icon]->setVisible( false);
   }
}

void StatusDisplayQt::showMessage(const QString & message, IconType type)
{
   if ( ! m_timer->isActive())
   {
      /* nothing currently shown. Show this message */
      m_messageArea->setText( message);
      m_messageAreaIcon->setPixmap( pixmapForType(type));

      m_timer->start( SHOW_MESSAGE_DURATION_MS);
   }
   else
   {
      /* enqueue message. It will be printed later */
      m_messages << new RichMessage( type, message);
   }
}

void StatusDisplayQt::showBlockingMessage(const QString & message, StatusDisplay::IconType /*type*/)
{
   QMessageBox::warning( nullptr, QObject::tr("Warning"), message);
}

QPixmap StatusDisplayQt::pixmapForType( StatusDisplay::IconType type)
{
   QPixmap * pixmap = m_iconTable.value( type, nullptr);

   return (pixmap != nullptr) ? *pixmap : QPixmap();
}


