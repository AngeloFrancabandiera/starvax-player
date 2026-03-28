#include "ActionListPainter.h"

#include <QStringList>
#include <QWidget>
#include <QTextOption>
#include <QVariant>
#include "qfileinfo.h"
#include "qrgb.h"
#include "testableAssert.h"
#include "ApplicationSettings.h"

/******************************  settings ******************************/
// background colors
#define  SELECTION_BG_COLOR     QColor( 240, 255, 180)
#define  ACTIVE_TRACK_BG_COLOR  QColor( 220, 250, 255)

// text colors
#define  SELECTED_TEXT_COLOR    QColor(255,128,0)
#define  ACTIVE_TEXT_COLOR      QColor(0, 128, 0)

// difference in font size between active and non-active items
#define ACTIVE_FONT_SIZE_DIFFERENCE  3

/***********************************************************************/
ActionListPainter::ActionListPainter(const ApplicationSettings & settings) :
   m_settings( settings)
{
   resetStyle();
   m_painter = nullptr;
   m_target = nullptr;
}

void ActionListPainter::setPainter(QPainter *painter , QWidget *parentWidget)
{
   m_painter = painter;
   m_painter->setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
   m_target = parentWidget;
}

void ActionListPainter::resetStyle()
{
   // text colors
   m_colActive     = ACTIVE_TEXT_COLOR;
   m_colSelection  = SELECTED_TEXT_COLOR;

   // background colors
   m_bgSelection = SELECTION_BG_COLOR;
   m_bgActive    = ACTIVE_TRACK_BG_COLOR;

   // set painting objects
   int defaultFontSize = m_settings.getSidePanelsFontSize();
   m_fntDefault.setBold( false );
   m_fntDefault.setItalic( false );
   m_fntDefault.setPointSize( defaultFontSize );

   m_fntActive.setBold( true );
   m_fntActive.setItalic( true );
   m_fntActive.setPointSize( defaultFontSize + ACTIVE_FONT_SIZE_DIFFERENCE );
}

void ActionListPainter::onPanelFontSizeChanged(int ptSize)
{
   m_fntDefault.setPointSize( ptSize );
   m_fntActive.setPointSize( ptSize + ACTIVE_FONT_SIZE_DIFFERENCE );
}

void ActionListPainter::paintRegularItem(const QRect &draw_area,
                                         const QVariant &displayData, int position)
{
   T_ASSERT( m_target != nullptr);
   m_painter->setFont( m_fntDefault );

   QBrush textColor = m_target->palette().windowText();
   m_painter->setPen( QPen(textColor, 1) );

   paintItem( draw_area, displayData, position );
}

void ActionListPainter::paintSelectedItem(const QRect &draw_area,
                                           const QVariant &displayData, int position)
{
   m_painter->fillRect( draw_area, m_bgSelection );
   m_painter->setFont( m_fntDefault );
   m_painter->setPen( QPen(m_colSelection) );

   paintItem( draw_area, displayData, position );
}

void ActionListPainter::paintActiveItem(const QRect &draw_area,
                                         const QVariant &displayData, int position)
{
   m_painter->fillRect( draw_area, m_bgActive );
   m_painter->setFont( m_fntActive );
   m_painter->setPen( QPen(m_colActive) );

   paintItem( draw_area, displayData, position );
}

/**
 * @brief minimal default behavior to paint an item.
 *    Subclasses must redefine the paint business.
 */
void ActionListPainter::paintItem(const QRect &draw_area, const QVariant &displayData, int position)
{
   QString displayString = displayData.toString();

   if (displayString == QString() )
   {
      displayString = " --- ";
   }

   m_painter->drawText( draw_area,
                        QString("%1. %2").arg(position).arg(displayString));
}


