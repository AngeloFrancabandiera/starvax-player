#ifndef MEDIATOOLBAR_H
#define MEDIATOOLBAR_H

#include <QObject>
#include <QAction>

#include "IF_MediaEngineInterface.h"
#include "AbstractMediaSource.h"

/**
 * @brief This class is a utility to enable or disable some actions
 * in response to the state of a multimedia file or a picture being
 * shown or hidden. It does not metter where these actions are placed.
 */
class MediaToolbar : public QObject
{
   Q_OBJECT
public:
   MediaToolbar( QAction *playAction, QAction *pauseAction,
                 QAction * rewindAction, QAction * stopAction, QObject *parent) :
      QObject(parent),
      m_playAction(playAction),
      m_pauseAction(pauseAction),
      m_rewindAction( rewindAction),
      m_stopAction( stopAction),
      m_isPictureLoaded( false),
      m_isPictureVisible( false)
   {
   }

   virtual ~MediaToolbar() override {}

public slots:

   void onPictureShowChanged( bool isShown)
   {
      m_isPictureVisible = isShown;
   }

   void onMediaChanged( const AbstractMediaSource & picture)
   {
      /* This is also called for non-picture files. In this case,
       * the cast to QPixmap return NULL object */
      m_isPictureLoaded = ((picture.mediaKind() == AbstractMediaSource::PICTURE_LINE_A) ||
                           (picture.mediaKind() == AbstractMediaSource::PICTURE_LINE_B) );
   }

private:
   QAction *m_playAction;
   QAction *m_pauseAction;
   QAction *m_rewindAction;
   QAction *m_stopAction;

   bool m_isPictureLoaded;
   bool m_isPictureVisible;
};

#endif // MEDIATOOLBAR_H
