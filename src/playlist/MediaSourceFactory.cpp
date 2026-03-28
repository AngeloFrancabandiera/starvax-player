#include "MediaSourceFactory.h"
#include "AudioVideoSource.h"
#include "PictureSource.h"
#include "supported_files.h"
#include "testableAssert.h"

#include <QFileInfo>
#include <QUrl>
#include <QPixmap>

MediaSourceFactory::MediaSourceFactory()
{
}


AbstractMediaSource *
MediaSourceFactory::buildMediaSource( const QString &fullPath,
                                      const QString &stringId,
                                      AbstractMediaSource::MediaKind mediaKind)
{
   AbstractMediaSource * mediaSource = nullptr;
   QString fileExt = QFileInfo( fullPath).suffix().toLower();

   if (PICTURE_SUPPORTED_FORMATS.contains( fileExt))
   {
      /* build a picture object */
      QPixmap * pixmap = new QPixmap( fullPath);
      T_ASSERT (pixmap != nullptr);
      mediaSource = new PictureSource( fullPath, stringId, mediaKind, *pixmap);
   }
   else if (MEDIA_TRACKS_SUPPORTED_FORMATS.contains( fileExt))
   {
      /* build an audio/video object */
      QVariant * nullContent = new QVariant();
      mediaSource = new AudioVideoSource( fullPath, stringId,
                                          mediaKind,
                                          *nullContent);
   }

   T_ASSERT (mediaSource != nullptr);

   return mediaSource;
}
