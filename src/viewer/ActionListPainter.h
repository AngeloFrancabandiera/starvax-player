#ifndef CPLAYLISTPAINTER_H
#define CPLAYLISTPAINTER_H


#include <QPainter>
#include "qcolor.h"
#include "qfont.h"
#include "qbrush.h"

class ApplicationSettings;


class ActionListPainter
{
public:
   explicit ActionListPainter(const ApplicationSettings & settings);
   virtual ~ActionListPainter() {}

   void setPainter( QPainter *painter, QWidget *parentWidget );

   void paintRegularItem( const QRect &draw_area,
                          const QVariant &displayData, int position);
   void paintSelectedItem( const QRect &draw_area,
                           const QVariant &displayData, int position);
   void paintActiveItem(const QRect &draw_area,
                        const QVariant &displayData, int position);
   void resetStyle();

   /* to be called when panel font size changes */
   void onPanelFontSizeChanged(int ptSize);

protected:
   /**
    * child classes must redefine paint behavior
    */
   virtual void paintItem(const QRect &draw_area, const QVariant &displayData, int position);

protected:

   QPainter *painter()
   {
      return m_painter;
   }

   QWidget *targetWidget() {
      return m_target;
   }

private:
   const ApplicationSettings & m_settings;
   QFont   m_fntDefault;
   QFont   m_fntActive;
   QColor  m_colActive;
   QColor  m_colSelection;
   QColor  m_bgDefault;
   QColor  m_bgActive;
   QColor  m_bgSelection;

   QPainter *m_painter;
   QWidget *m_target;
};

#endif // CPLAYLISTPAINTER_H
