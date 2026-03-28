#ifndef ABSTRACTMEDIASOURCE_H
#define ABSTRACTMEDIASOURCE_H

#include <QString>
#include <QVariant>

/** @class AbstractMediaSource
 *
 * This is an abstract interface only meant to create a media
 * source, defined by its file full path.
 * Model-View logic will deal with this abstract type.
 * Concrete subclesses will wrap the plugin specific media source.
 */

class AbstractMediaSource : public QObject
{
   Q_OBJECT
public:
   enum MediaKind {
      KIND_NONE = 0,
      AUDIO_VIDEO_LINE_A,
      AUDIO_VIDEO_LINE_B,
      PICTURE_LINE_A,
      PICTURE_LINE_B
   };

   AbstractMediaSource() {}
   ~AbstractMediaSource() override {}

   /** full path of media file */
   virtual QString fileName() const = 0;

   /** a string that identifies the resource within the application.
    *  Implementation is in charge of the model, and it's normally the
    *  base name of file, unless there are confilcts. */
   virtual QString stringId() const = 0;

   /**
    * binary media data to be played. Format is plugin specific,
    * but must be a meta-type
    */
   virtual QVariant mediaContent() const = 0;

   /**
    * @return the kind of media
    */
   virtual MediaKind mediaKind() const = 0;
};

Q_DECLARE_METATYPE( const AbstractMediaSource *)


class NullMediaSource : public AbstractMediaSource
{
public:
   NullMediaSource() :
      AbstractMediaSource()
   {}

   // AbstractMediaSource interface
public:
   QString fileName() const override
   {
      return QString();
   }

   QString stringId() const override
   {
      return QString();
   }

   QVariant mediaContent() const override
   {
      return QVariant();
   }

   MediaKind mediaKind() const override
   {
      return AbstractMediaSource::KIND_NONE;
   }
};


#endif // ABSTRACTMEDIASOURCE_H
