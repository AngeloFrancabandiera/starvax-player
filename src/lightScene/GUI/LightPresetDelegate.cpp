#include "LightPresetDelegate.h"
#include "LightPresetData.h"
#include "LightPresetModel.h"
#include "LightSetEditor.h"

#include <QEvent>
#include <QCommonStyle>
#include <QScreen>
#include <QApplication>
#include <QWidget>

#include "modelViewRules.h"


LightPresetDelegate::LightPresetDelegate(int numberOfChannels, QObject *parent) :
   QStyledItemDelegate(parent),
   m_numOfChannels(numberOfChannels)
{
   /* the editor will appare in fixed position; such position comes from
    * screen size. */
   QSize screenSize = qApp->screens().at(0)->size();
   m_editorPosX = screenSize.width() / 4;
   m_editorPosY = screenSize.height() / 2;
}


QWidget *LightPresetDelegate::createEditor(QWidget * /*parent*/, const QStyleOptionViewItem &/*option*/,
                                           const QModelIndex &/*index*/) const
{
   LightSetEditor *editor = new LightSetEditor( m_numOfChannels, nullptr);
   editor->setWindowFlags(Qt::Dialog);  /* in order to cover eventual playbar */

   connect( editor, & LightSetEditor::editComplete, this, & LightPresetDelegate::onEditComplete);
   editor->topLevelWidget()->setWindowFlags( editor->topLevelWidget()->windowFlags() | Qt::FramelessWindowHint);

   return editor;
}

void LightPresetDelegate::setEditorData(QWidget * editor, const QModelIndex &index) const
{
   const LightPresetData & itemData =
         index.data(modelViewRules::Binary).value< LightPresetData >();

   LightSetEditor *presetEditor = dynamic_cast<LightSetEditor *>(editor);
   presetEditor->setName( itemData.name());
   presetEditor->setFadeMs( itemData.fadeTime());
   presetEditor->setLevelList( itemData.levelList());
}

void LightPresetDelegate::setModelData(QWidget * editor, QAbstractItemModel * model,
                                       const QModelIndex & index) const
{
   LightSetEditor *presetEditor = dynamic_cast<LightSetEditor *>(editor);
   LightPresetModel *presetModel = dynamic_cast<LightPresetModel *>(model);

   const LightPresetData & itemData =
         index.data(modelViewRules::Binary).value<LightPresetData>();
   QString oldName = itemData.name();

   /* if name has changed, item must be renamed before updating values */
   presetModel->beginUndoMacro("set item data");
   presetModel->renamePreset( oldName, presetEditor->name());
   presetModel->updatePreset( presetEditor->name(), presetEditor->fadeMs(),
                              presetEditor->levelList() );
   presetModel->endUndoMacro();
}

void LightPresetDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem & /*option*/,
                                               const QModelIndex & /*index*/) const
{
   /* size is (1,1) so minimum allowed will be used */
   QRect editorRect( m_editorPosX, m_editorPosY, 1, 1);
   editor->setGeometry( editorRect );
}

void LightPresetDelegate::onEditComplete( QWidget * editor)
{
   commitData( editor);
   closeEditor( editor);
}



void LightPresetDelegate::destroyEditor(QWidget * editor, const QModelIndex & /*index*/) const
{
   delete editor;
}

