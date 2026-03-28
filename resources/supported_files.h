#ifndef SUPPORTED_FILES_H
#define SUPPORTED_FILES_H

#include <QStringList>

/*****************************************************************************/
/*  This file contains the information about supported file extentions       */
/*****************************************************************************/

/***  supported file extentions for Scripts  ***/
#define  SCRIPT_SUPPORTED_FORMATS   (QStringList()<<"htm"<<"html"<<"txt")

#define  SCRIPT_DLG_FILTER          "ritch text (*.htm *.html);; text (*.txt)"
/***********************************************/

/** still image formats */
const QStringList PICTURE_SUPPORTED_FORMATS = (QStringList()<<"png"<<"jpg"<<"jpeg"<<"gif"<<"svg");

/***  supported file extentions for media tracks  ***/
const QStringList MEDIA_TRACKS_SUPPORTED_FORMATS  =
      (QStringList()<<"mp3"<<"wav"<<"au"<<"avi"<<"mpg"<<"mp4"<<"m4v"<<"flv"<<"mkv"<<"mts"<<"mov"<<"webm"<<"ogg"<<"ogv"<<"opus");

/** this is used by GUI dialog to show allowed files */
#define  MEDIA_TRACKS_DLG_FILTER     "Media (*.mp3 *.wav *.au *.avi *.mpg *.mp4 *.flv *.mkv *.mts *.mov *.m4v *.webm *.png *.jpg *.jpeg *.gif *.ogg *.ogv *.opus *.svg)"
/***********************************************/

/***  supported file extentions for Shows  ***/
#define  SHOWS_SUPPORTED_FORMATS        (QStringList()<<"mts"<<"meteor")

#define  SHOWS_DLG_FILTER               "Me.Te.Or. Shows (*.meteor *.mts)"
/***********************************************/

/***  supported file extentions for Style files  ***/
#define  STYLE_SHEET_EXT                QString("qss")

#define  STYLES_DLG_FILTER              "qt Stylesheet ( *.qss )"
/***********************************************/

#endif // SUPPORTED_FILES_H
