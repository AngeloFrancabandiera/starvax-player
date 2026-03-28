#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H

#include <QStyledItemDelegate>



class HtmlDelegate : public QStyledItemDelegate
{
   Q_OBJECT
public:
   HtmlDelegate( QObject * parent = nullptr);

protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const override;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const override;
};

#endif // HTMLDELEGATE_H
