#include "LightsetPainter.h"
#include "LightPresetData.h"
#include "QVariant"

LightsetPainter::LightsetPainter(const ApplicationSettings & settings) :
   ActionListPainter(settings)
{
}


void LightsetPainter::paintItem( const QRect &draw_area,
                                 const QVariant &displayData,
                                 int position)
{
   QTextOption txt_opt;
   txt_opt.setWrapMode( QTextOption::NoWrap );

   const LightPresetData & lightData = displayData.value< LightPresetData>();

   painter()->drawText( draw_area.adjusted(5,0, 0, 0),
                        formatDisplayText( position, & lightData ), txt_opt );
}


QString LightsetPainter::formatDisplayText( int position, const LightPresetData *preset)
{
   return QString( "[%1] - %2    [%3 sec]" ).
          arg(position + 1).arg(preset->name()).arg(static_cast<double>(preset->fadeTime())/1000., 3);
}
