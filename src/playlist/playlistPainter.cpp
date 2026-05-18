#include "playlistPainter.h"
#include "QVariant"
#include "ApplicationIcon.h"
#include "AbstractMediaSource.h"
#include "testableAssert.h"

#include <QMap>

namespace  {
QMap< AbstractMediaSource::MediaKind, QPixmap *> TypeToIconMap;

}  // namespace


PlaylistPainter::PlaylistPainter(const ApplicationSettings &settings) :
   ActionListPainter( settings)
{
   /* TBD: all decks use the same icon */
   m_pictureIcon = QPixmap( IconPath("picture_A.png")).scaled(16,16);
   m_soundIcon = QPixmap( IconPath("sound_A.png")).scaled(16,16);

   TypeToIconMap.insert( AbstractMediaSource::AUDIO_VIDEO, & m_soundIcon);
   TypeToIconMap.insert( AbstractMediaSource::PICTURE, & m_pictureIcon);
}


void PlaylistPainter::paintItem( const QRect &draw_area,
                                 const QVariant &displayData,
                                 int position)
{
   const AbstractMediaSource * media =
         displayData.value<const AbstractMediaSource *>();
   T_ASSERT( media);

   QString displayText = media->stringId();
   QPixmap * pixmap = TypeToIconMap.value( media->mediaKind(), nullptr);

   int labelOffsetX = (pixmap != nullptr) ? 20 : 4;

   if (pixmap != nullptr)
   {
      painter()->drawPixmap( draw_area.x() + 2, draw_area.y() + 2, *pixmap);
   }

   QTextOption txt_opt;
   txt_opt.setWrapMode( QTextOption::NoWrap );

   painter()->drawText( draw_area.adjusted(labelOffsetX,0, 0, 0),
                        formatDisplayText( position, displayText ), txt_opt );
}

/**
 * @param position - item list number (1 based)
 * @param displayText - base name of full media path
 * @return  text to show in view.
 */
QString PlaylistPainter::formatDisplayText( int position, const QString &displayText )
{
   return QString( "[%1] - %2" ).arg(position + 1).arg(displayText);
}
