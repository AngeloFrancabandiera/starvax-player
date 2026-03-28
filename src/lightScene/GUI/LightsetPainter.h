#ifndef LIGHTSETPAINTER_H
#define LIGHTSETPAINTER_H

#include "ActionListPainter.h"

class LightPresetData;

class LightsetPainter : public ActionListPainter
{
public:
   LightsetPainter(const ApplicationSettings &settings);

protected:
   void paintItem( const QRect &draw_area, const QVariant &displayData, int position);

private:
   QString formatDisplayText(int position, const LightPresetData *preset );
};

#endif // LIGHTSETPAINTER_H
