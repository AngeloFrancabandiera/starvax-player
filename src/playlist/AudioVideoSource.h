#ifndef MEDIASOURCEQT_H
#define MEDIASOURCEQT_H

#include "AbstractMediaSource.h"

/**
 * This class holds the location of an Audio/Video source (file path)
 * and possibly a binary content.
 * Field 'mediaContent' in constructor may be an instance of QMediaContent
 * if Qt Multimedia implementation is used
 */
class AudioVideoSource : public AbstractMediaSource
{
public:
   AudioVideoSource( const QString &fileName, const QString &stringId,
                     AbstractMediaSource::MediaKind mediaKind,
                     QVariant & mediaContent);

   // AbstractMediaSource interface
public:
   QString fileName() const override;
   QString stringId() const override;
   QVariant mediaContent() const override;
   MediaKind mediaKind() const override;

private:
   QString m_fileName;
   QString m_stringId;
   QVariant & m_mediaContent;
   AbstractMediaSource::MediaKind m_mediaKind;
};

#endif // MEDIASOURCEQT_H
