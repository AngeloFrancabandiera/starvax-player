#ifndef PICTURESOURCE_H
#define PICTURESOURCE_H

#include "AbstractMediaSource.h"


class PictureSource : public AbstractMediaSource
{
public:
   PictureSource( const QString &fileName, const QString &stringId,
                  AbstractMediaSource::MediaKind mediaKind,
                  QPixmap & pixmap);

   ~PictureSource() override;

   // AbstractMediaSource interface
public:
   QString fileName() const override;
   QString stringId() const override;
   QVariant mediaContent() const override;
   MediaKind mediaKind() const override;

private:
   QString m_fileName;
   QString m_stringId;
   AbstractMediaSource::MediaKind m_mediaKind;
   QPixmap & m_pixmap;
};

#endif // PICTURESOURCE_H
