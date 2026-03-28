#include "SequencerErrorTableWidget.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMouseEvent>
#include <QVariant>

#include "testableAssert.h"
#include "HtmlDelegate.h"

namespace  {
/** firts column: line number of the error */
const int LINE_NUMBER_COLUMN = 0;

/** second column: description */
const int DESCRIPTION_COLUMN = 1;
}

SequencerErrorTableWidget::SequencerErrorTableWidget(QWidget *parent) :
   QTableWidget( parent)
{
   setColumnCount( 2);
   setRowCount(0);

   setHorizontalHeaderLabels( QStringList() << "Line" << "Description");
   verticalHeader()->hide();

   setItemDelegate( new HtmlDelegate(this));
}

void SequencerErrorTableWidget::clearErrorList()
{
   int count = rowCount();

   for (int i=0; i < count; i++)
   {
      removeRow( 0);
   }

   hide();
}

void SequencerErrorTableWidget::addError( int line, const QString & description)
{
   QTableWidgetItem * lineNumberItem = new QTableWidgetItem( QTableWidgetItem::Type);
   QTableWidgetItem * descriptionItem = new QTableWidgetItem( QTableWidgetItem::Type);
   lineNumberItem->setData( Qt::EditRole, line);
   descriptionItem->setData( Qt::EditRole, description);

   lineNumberItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable);
   descriptionItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable);

   setRowCount( rowCount() + 1);

   setItem( rowCount() - 1, LINE_NUMBER_COLUMN, lineNumberItem);
   setItem( rowCount() - 1, DESCRIPTION_COLUMN, descriptionItem);

   /* keep this order: column before row */
   resizeColumnsToContents();
   resizeRowsToContents();

   show();
}


void SequencerErrorTableWidget::mousePressEvent(QMouseEvent *event)
{
   QTableWidget::mousePressEvent( event);

   /* emit a signal that a line has been clicked */
   bool convOk = false;
   QTableWidgetItem * cleckedItem = item( rowAt( event->position().y()), 0);

   /* check if double clicked on a valid event */
   if( cleckedItem != nullptr)
   {
      int editorLine = cleckedItem->text().toInt( & convOk);

      if (convOk)
      {
         emit highlighLineRequest( editorLine);
      }
   }
}

