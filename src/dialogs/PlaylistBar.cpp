#include "PlaylistBar.h"
#include "PicturePlaybar.h"
#include "AudioVideoPlayBar.h"
#include "MediaListModel.h"
#include "testableAssert.h"


PlaylistBar::PlaylistBar( AudioVideoPlayBar & audioVideoPlaybar,
                          PicturePlaybar & picturePlaybar,
                          MediaListModel & mediaList,
                          QWidget * parent) :
   QStackedWidget( parent),
   m_audioVideoPlaybar( audioVideoPlaybar),
   m_picturePlaybar( picturePlaybar),
   m_mediaList(mediaList)
{
   addWidget( &m_audioVideoPlaybar);
   addWidget( &m_picturePlaybar);

   setMinimumHeight( m_audioVideoPlaybar.height());
   setVisible( false);
}

PlaylistBar::~PlaylistBar()
{
}

// TO-DO bad RTTI (this function and the next)
#include "AudioVideoSource.h"
#include "PictureSource.h"
void PlaylistBar::onActiveRowChanged(int row)
{
   /* select the correct widget, according to row */
   if ( (row >= 0) && (row < m_mediaList.rowCount()))
   {
      QModelIndex modelIndex = m_mediaList.index(row);
      const AbstractMediaSource * source = m_mediaList.mediaData( modelIndex);

      const AudioVideoSource * audVidSrc = dynamic_cast<const AudioVideoSource *>(source);
      if (audVidSrc != nullptr)
      {
         setCurrentWidget( &m_audioVideoPlaybar);
         setVisible( true);
      }
      else
      {
         const PictureSource * pictSource = dynamic_cast<const PictureSource *>(source);
         if (pictSource != nullptr)
         {
            setCurrentWidget( &m_picturePlaybar);
            setVisible( true);
         }
         else
         {
            T_ASSERT (false);
         }
      }
   }
   else
   {
      /* this happens when active item is deleted.
       * In this case, 'row' is -1 */
      setVisible( false);
   }
}

void PlaylistBar::onRequestToPlay()
{
   // bad RTTI
   if (currentWidget() == (& m_audioVideoPlaybar))
   {
      m_audioVideoPlaybar.play();
   }
   else if (currentWidget() == (& m_picturePlaybar))
   {
      m_picturePlaybar.showPicture();
   }
}

