#ifndef MEDIASOURCEFACTORY_H
#define MEDIASOURCEFACTORY_H

#include <QObject>

#include "AbstractMediaSource.h"

/**
 * virtual class to instantiate an AbstractMediaSource
 */
class MediaSourceFactoryInterface
{
public:
   explicit MediaSourceFactoryInterface() {}
   virtual  ~MediaSourceFactoryInterface() {}

   virtual AbstractMediaSource * buildMediaSource( const QString & fullPath,
                                                   const QString & stringId,
                                                   AbstractMediaSource::MediaKind mediaKind) = 0;
};

/**
 * Concrete class that instantiates a media source that has
 * a 'Qt multiemdia content' object
 */
class MediaSourceFactory : public MediaSourceFactoryInterface
{
public:
   explicit MediaSourceFactory();
   virtual  ~MediaSourceFactory() {}

   virtual AbstractMediaSource * buildMediaSource( const QString & fullPath,
                                                   const QString & stringId,
                                                   AbstractMediaSource::MediaKind mediaKind);
};

#endif // MEDIASOURCEFACTORY_H
