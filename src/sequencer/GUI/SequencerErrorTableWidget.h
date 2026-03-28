#ifndef SEQUENCERERRORTABLEWIDGET_H
#define SEQUENCERERRORTABLEWIDGET_H

#include <QTableWidget>

class SequencerErrorTableWidget : public QTableWidget
{
   Q_OBJECT
public:
   SequencerErrorTableWidget( QWidget * parent = nullptr);

   void clearErrorList();

   void addError(int line, const QString & description);

signals:
   /* user clicked on a line. Request to highlight it in editor */
   void highlighLineRequest( int line);

   // QWidget interface
protected:
   void mousePressEvent(QMouseEvent *event) override;
};

#endif // SEQUENCERERRORTABLEWIDGET_H
