#ifndef OPERATIVEOWNDELEGATE_H
#define OPERATIVEOWNDELEGATE_H

#include <QStyledItemDelegate>
class OpenWebNetEngine_IF;

/**
 * @brief The OperativeOwnDelegate class id the delegate of Open Web Net
 * items in operative mode. The 'editor' is actually a set of buttons
 * to activate lights
 */
class OperativeOwnDelegate : public QStyledItemDelegate
{
   Q_OBJECT
public:
   OperativeOwnDelegate( OpenWebNetEngine_IF & ownEngine,
                         QObject * parent = nullptr);

private:
   OpenWebNetEngine_IF & m_ownEngine;

   // QAbstractItemDelegate interface
public:
   virtual QWidget *createEditor( QWidget * parent, const QStyleOptionViewItem & option,
                                  const QModelIndex & index) const;
   virtual void setModelData( QWidget * editor, QAbstractItemModel * model,
                              const QModelIndex & index) const;
};

#endif // OPERATIVEOWNDELEGATE_H
