#ifndef PlaylistPainter_H
#define PlaylistPainter_H

#include "ActionListPainter.h"
#include "QPixmap"

class PlaylistPainter : public ActionListPainter
{
public:
   PlaylistPainter(const ApplicationSettings &settings);

protected:
   void paintItem(const QRect &draw_area, const QVariant &displayData, int position);

private:
   QString formatDisplayText(int position, const QString &displayText );
   QPixmap m_pictureIcon;
   QPixmap m_soundIcon;
};

#endif // PlaylistPainter_H
