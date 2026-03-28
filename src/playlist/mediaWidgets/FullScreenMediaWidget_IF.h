#ifndef FULLSCREENMEDIAWIDGET_IF_H
#define FULLSCREENMEDIAWIDGET_IF_H

class QPixmap;
class QVideoWidget;
class StillPictureWidget;


/**
 * This is the interface used by media engine to display in
 * full screen a video or a still image
 */
class FullScreenMediaWidget_IF
{
public:
   virtual ~FullScreenMediaWidget_IF() {}

   virtual void attachWidgets( QVideoWidget *videoWidget,
                               StillPictureWidget *pictureWidget) = 0;

   /** set the Pixmap that this widget can display.
    * There is no equivalent function for video because it is
    * managed by backend */
   virtual void setPixmap( const QPixmap & pixmap) = 0;

   /** request to show the widget that holds video */
   virtual void showVideo() = 0;

   /** request to show the widget that holds still image. */
   virtual void showPicture() = 0;

   /** request to hide still image widget */
   virtual void hidePicture() = 0;

   /** request to toggle visibility for still image widget
    *  @return TRUE if picture is visible after being toggled */
   virtual bool togglePictureVisibility() = 0;

   /** request to hide video widget */
   virtual void hideVideo() = 0;

   /** request to hide both video and picture widget */
   virtual void hideAll() = 0;

   /** set or unset this window as 'Always on top'. This affects both picture and video */
   virtual void setOnTop( bool onTop) = 0;

};

#endif // FULLSCREENMEDIAWIDGET_IF_H
