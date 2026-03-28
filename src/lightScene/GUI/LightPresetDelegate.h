#ifndef LIGHTPRESETDELEGATE_H
#define LIGHTPRESETDELEGATE_H

#include <QStyledItemDelegate>

class LightPresetDelegate : public QStyledItemDelegate
{
   Q_OBJECT
public:
   explicit LightPresetDelegate(int numberOfChannels, QObject *parent = nullptr);

   /* QAbstractItemDelegate interface */
public:
   QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;
   void destroyEditor(QWidget * editor, const QModelIndex & index) const override;
   void setEditorData(QWidget *editor, const QModelIndex &index) const override;
   void setModelData(QWidget *editor, QAbstractItemModel *model,
                     const QModelIndex &index) const override;
   void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

private slots:
   void onEditComplete( QWidget *editor);

private:
   int m_numOfChannels;
   int m_editorPosX;
   int m_editorPosY;

};

#endif // LIGHTPRESETDELEGATE_H
