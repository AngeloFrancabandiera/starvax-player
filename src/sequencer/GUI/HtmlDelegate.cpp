#include "HtmlDelegate.h"
#include <QPainter>
#include <QTextDocument>

HtmlDelegate::HtmlDelegate(QObject *parent) :
   QStyledItemDelegate( parent)
{

}

void HtmlDelegate::paint( QPainter* painter, const QStyleOptionViewItem & option,
                          const QModelIndex &index) const
{
   QStyleOptionViewItem options = option;
   initStyleOption(&options, index);

   painter->save();

   QTextDocument doc;
   doc.setHtml(options.text);

   options.text = "";
   options.widget->style()->drawControl( QStyle::CE_ItemViewItem,
                                         &options, painter);

   painter->translate( options.rect.left(), options.rect.top());
   QRect clip( 0, 0, options.rect.width(), options.rect.height());
   doc.drawContents( painter, clip);

   painter->restore();
}

QSize HtmlDelegate::sizeHint ( const QStyleOptionViewItem & option,
                               const QModelIndex & index ) const
{
   QStyleOptionViewItem options = option;
   initStyleOption(&options, index);

   QTextDocument doc;
   doc.setHtml(options.text);
   return QSize( static_cast<int>(doc.size().width()),
                 static_cast<int>(doc.size().height()));
}
