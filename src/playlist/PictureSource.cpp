#include "PictureSource.h"
#include <QPixmap>


PictureSource::PictureSource( const QString & fileName,
                              const QString & stringId, MediaKind mediaKind,
                              QPixmap & pixmap) :
   m_fileName( fileName),
   m_stringId( stringId),
   m_mediaKind( mediaKind),
   m_pixmap( pixmap)
{
}

PictureSource::~PictureSource()
{
}


QString PictureSource::fileName() const
{
   return m_fileName;
}

QString PictureSource::stringId() const
{
   return m_stringId;
}

QVariant PictureSource::mediaContent() const
{
   return QVariant(m_pixmap);
}

AbstractMediaSource::MediaKind PictureSource::mediaKind() const
{
   return m_mediaKind;
}
