#ifndef LIGHTPRESETMODEL_H
#define LIGHTPRESETMODEL_H

#include "ExtAbstractListModel.h"

class LightPresetData;
class LightPresetFactory;
class QUndoStack;

namespace LightPresetModelCommands {
class RenamePresetCommand;
class AddNewPresetCommand;
class UpdateExistingPresetCommand;
class RemoveRowCommand;
class MoveRowCommand;
class SwapCommand;
}


class LightPresetModel : public ExtAbstractListModel
{
   Q_OBJECT

   friend class LightPresetModelCommands::RenamePresetCommand;
   friend class LightPresetModelCommands::AddNewPresetCommand;
   friend class LightPresetModelCommands::UpdateExistingPresetCommand;
   friend class LightPresetModelCommands::RemoveRowCommand;
   friend class LightPresetModelCommands::MoveRowCommand;
public:
   explicit LightPresetModel( int numberOfChannels, LightPresetFactory *presetFactory,
                              QObject *parent = nullptr);
   ~LightPresetModel() override;

public :
   int rowCount(const QModelIndex &parent = QModelIndex()) const override;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
   bool setData(const QModelIndex & index, const QVariant & value, int role) override;
   QVariant headerData(int, Qt::Orientation, int ) const override;
   bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() ) override;
   QMimeData *mimeData(const QModelIndexList & indexes) const override;
   QStringList mimeTypes() const override;

   Qt::ItemFlags flags(const QModelIndex &index) const override;

   void updatePreset(const QString &name, int fadeMs, const QList<double> &targetValues);
   void updatePreset(const LightPresetData &preset);

   void renamePreset( const QString &oldName, const QString &newName );

   QModelIndex searchByName(const QString &targetName) const;

   bool moveRows(const QModelIndex &, int sourceRow, int count,
                 const QModelIndex &, int destinationChild) override;

   int numberOfChannels() {
      return m_numberOfChannels;
   }

   /** undo last operation on internal model */
   void undo() override;
   /** redo last operation on internal model */
   void redo() override;

   /** start a set of operation that will be undone together */
   void beginUndoMacro( const QString & name) override;
   /** end a set of operation that will be undone together */
   void endUndoMacro() override;

   /** move a set of items (not necessarily contiuos) and store them
    *   continuosly at a given index
    * @param selectedIndexes holds the indexes that have been selected
    * @param targetRow is the row at which the first item will be placed after re-arrangement
    */
   void rearrangeItems(QList<QModelIndex> & selectedIndexes, int targetRow ) override;

signals:
   void canRedoChanged(bool canRedo);
   void canUndoChanged(bool canUndo);
   void cleanChanged(bool clean);

public slots:
      void createPresetWithValues(const QList<double> &values);

private:
   LightPresetFactory *m_presetFactory;
   QList<LightPresetData> m_presetDataStore;
   int m_numberOfChannels;
   QUndoStack * m_undoStack;

private:
   void assertValidPosition(int position) const;
   QList<double> fillTargetValues(const QList<double> &targetValues);
   QString newPresetName() const;
   bool existsPresetWithName( const QString & name) const;
};

#endif // LIGHTPRESETMODEL_H
