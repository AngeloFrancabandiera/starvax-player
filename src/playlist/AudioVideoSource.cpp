#include "AudioVideoSource.h"
#include "testableAssert.h"

AudioVideoSource::AudioVideoSource( const QString &fileName, const QString &stringId,
                                    MediaKind mediaKind,
                                    QVariant & mediaContent) :
   m_fileName(QString(fileName)),
   m_stringId(QString(stringId)),
   m_mediaContent(mediaContent),
   m_mediaKind( mediaKind)
{
}


QString AudioVideoSource::fileName() const
{
   return m_fileName;
}

QString AudioVideoSource::stringId() const
{
   return m_stringId;
}

QVariant AudioVideoSource::mediaContent() const
{
   T_ASSERT_REP( false, "This implementation can't use 'AudioVideoSource::mediaContent()'");
   return QVariant();
}

AbstractMediaSource::MediaKind AudioVideoSource::mediaKind() const
{
   return m_mediaKind;
}

