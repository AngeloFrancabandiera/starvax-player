/*  This file is part of the KDE project
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), Nokia Corporation
    (or its successors, if any) and the KDE Free Qt Foundation, which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "swiftslider.h"

#include <QtGui/QMouseEvent>
#include <QStyle>
#include <QStyleOption>
#include <QKeyEvent>

#include "qtooltip.h"


SwiftSlider::SwiftSlider(Qt::Orientation orientation, QWidget * parent)
   : QSlider(orientation, parent)
{
   this->setTracking( true);
   this->setMouseTracking(true);
}

SwiftSlider::~SwiftSlider()
{
}

// Function copied from qslider.cpp
inline int SwiftSlider::pick(const QPoint &pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}

// Function copied from qslider.cpp and modified to make it compile
int SwiftSlider::pixelPosToRangeValue(int pos) const
{
   QStyleOptionSlider opt;
   initStyleOption(&opt);
   QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
   QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
   int sliderMin, sliderMax, sliderLength;

   if (orientation() == Qt::Horizontal)
   {
      sliderLength = sr.width();
      sliderMin = gr.x();
      sliderMax = gr.right() - sliderLength + 1;
   }
   else
   {
      sliderLength = sr.height();
      sliderMin = gr.y();
      sliderMax = gr.bottom() - sliderLength + 1;
   }

   return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                          sliderMax - sliderMin, opt.upsideDown);
}

// Based on code from qslider.cpp
void SwiftSlider::mousePressEvent(QMouseEvent *event)
{
   if (event->button() == Qt::LeftButton)
   {
      int rangeValue = mouseEventToRangeValue( event);

      if (rangeValue >= 0)
      {
         event->accept();

         setSliderPosition( rangeValue);
         triggerAction(SliderMove);
         setRepeatAction(SliderNoAction);
      }
      else
      {
         QSlider::mousePressEvent(event);
      }
   }
   else
   {
      QSlider::mousePressEvent(event);
   }
}

/**
 * @brief when mouse is hovered on slider, the timestamp related
 *        to that position is shown.
 *        If left button is pressed on move event, drag happens without
 *        time tooltip
 *
 * @param ev - mouse event
 *
 *************************************************************************/
void SwiftSlider::mouseMoveEvent(QMouseEvent *ev)
{
   QSlider::mouseMoveEvent(ev);

   int rangeValue = mouseEventToRangeValue(ev);

   if (rangeValue >= 0)
   {
      if (ev->buttons() & Qt::LeftButton)
      {
         // dragging
         setSliderPosition( rangeValue);
         triggerAction(SliderMove);
         setRepeatAction(SliderNoAction);
      }
      else
      {
         // hover without press
         QToolTip::showText( ev->globalPosition().toPoint(), formatTip((qint64)rangeValue), this);
      }
   }
   else
   {
      /* this happens when mouse is over the handle */
      QToolTip::showText( ev->globalPosition().toPoint(), formatHandleTip((qint64)value()), this);
   }
}

/**
 * change a mouse clicked position to a tick value of the slider
 *
 * @param ev - used to get mouse position
 *
 * @retval  tick value of the slider. If position is outside the slider,
 *          a negative value is returned
 *
 *************************************************************************/
int SwiftSlider::mouseEventToRangeValue(const QMouseEvent *ev)
{
   int rangeValue = -1;

   QStyleOptionSlider opt;
   initStyleOption(&opt);
   const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt,
                                                    QStyle::SC_SliderHandle, this);
   const QPoint center = sliderRect.center() - sliderRect.topLeft();
   // to take half of the slider off for the setSliderPosition call we use the center - topLeft

   if (!sliderRect.contains(ev->pos()))
   {
      rangeValue = pixelPosToRangeValue(pick(ev->pos() - center));
   }

   return rangeValue;
}

/**
 * child classes are supposed to cutomize position tooltip, but a
 * default behavior is implemented; (value itself is printed).
 *
 * @param value - in ticks of the slider
 * @return string to be shown as tooltip, in html format
 */
QString SwiftSlider::formatTip( qint64 value)
{
   return QString("%1").arg(value);
}

/**
 * same as ::SwiftSlider::formatTip, but called when mouse is OVERFLOW
 * the handle of the slider
 */
QString SwiftSlider::formatHandleTip(qint64 value)
{
   return QString("<b><i>%1</i></b>").arg(formatTip(value));
}

void SwiftSlider::focusInEvent(QFocusEvent * event)
{
   QSlider::focusInEvent( event);

   /* reset eventual key editing */
   keyEditedValue = QString();
}


/**
 * numeric keys can be used to type the value. Each key is cat
 * with keys previously pressed. Backspace can be used to fix value.
 * Toggle focus to reset key editing.
 */
void SwiftSlider::keyReleaseEvent(QKeyEvent * event)
{
   QSlider::keyReleaseEvent( event);

   bool updateValue = false;
   int key = event->key();

   if ((key >= Qt::Key_0) && (key <= Qt::Key_9))
   {
      keyEditedValue += QChar( key);
      updateValue = true;
   }
   else if (key == Qt::Key_Backspace)
   {
      /* remove last char, if any */
      keyEditedValue.chop(1);
      updateValue = true;
   }

   if (updateValue)
   {
      /* correctenss hopefully guaranteed by design */
      int value = keyEditedValue.toInt();
      // TBD ??? I can't find out why there is a factor 3 in the value
      //         that is actually set.
      setValue( value*3);
   }
}





