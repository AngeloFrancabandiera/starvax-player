#include <qstringlist.h>
#include <qmimedata.h>
#include <QUndoStack>
#include <QUndoCommand>
#include <QIODevice>
#include <QDataStream>

#include "LightPresetModel.h"
#include "LightPresetData.h"
#include "LightPresetFactory.h"
#include "modelViewRules.h"
#include "testableAssert.h"

#define DEFAULT_PRESET_NAME    QString("preset")
#define DEFAULT_TRNSITION_TIME    1000

namespace LightPresetModelCommands
{

class RenamePresetCommand : public QUndoCommand
{
public:
   RenamePresetCommand( LightPresetModel * owner,
                        const QString &oldName, const QString &newName) :
      QUndoCommand(),
      _owner( owner),
      _oldName( oldName),
      _newName( newName)
   {
   }

   // QUndoCommand interface
public:
   void redo() override
   {
      renamePreset( _oldName, _newName);
   }

   void undo() override
   {
      renamePreset( _newName, _oldName);
   }

private:
   /* this is used by both 'redo' and 'undo' */
   void renamePreset( const QString &oldName, const QString &newName)
   {
      QModelIndex indexOfExistingPreset = _owner->searchByName( oldName );

      if (indexOfExistingPreset != QModelIndex())
      {
         T_ASSERT( indexOfExistingPreset.row() < _owner->m_presetDataStore.length());

         int row = indexOfExistingPreset.row();
         _owner->m_presetDataStore[row].changeName( newName);

         emit _owner->layoutChanged();
      }
   }

private:
   LightPresetModel * _owner;
   QString _oldName;
   QString _newName;
};

class AddNewPresetCommand : public QUndoCommand
{
public:
   AddNewPresetCommand( LightPresetModel * owner,
                           int position,
                           const LightPresetData &preset ) :
      _owner( owner),
      _position( position),
      _newPreset( preset)
   {
   }

   // QUndoCommand interface
public:
   void redo() override
   {
      _owner->m_presetDataStore.insert( _position, _newPreset);
      emit _owner->layoutChanged();
   }

   void undo() override
   {
      if (_owner->m_presetDataStore.size() > 0)
      {
         _owner->m_presetDataStore.removeAt( _position);
         emit _owner->layoutChanged();
      }
   }

private:
   LightPresetModel * _owner;
   int _position;
   LightPresetData _newPreset;
};

/** search a preset with the name specified in constructor.
 * For such preset, all other data for such preset are replaced.  */
class UpdateExistingPresetCommand :public QUndoCommand
{
public:
   UpdateExistingPresetCommand( LightPresetModel * owner,
                                const LightPresetData & preset) :
      _owner( owner),
      _preset( preset)
   {
      QModelIndex index = _owner->searchByName( _preset.name());
      /* 'row' will always be the same at the time of the 'undo' if all
       * operations on the model are performed by undo-stack */
      _row = index.row();

      T_ASSERT( _row >= 0);
      T_ASSERT( _row < _owner->m_presetDataStore.size());
      _oldPreset = _owner->m_presetDataStore.at(_row);
   }

   // QUndoCommand interface
public:
   void redo() override
   {
      _owner->m_presetDataStore.replace( _row, _preset);
      emit _owner->layoutChanged();
   }

   void undo() override
   {
      _owner->m_presetDataStore.replace( _row, _oldPreset);
      emit _owner->layoutChanged();
   }

private:
   LightPresetModel * _owner;
   LightPresetData _preset;
   LightPresetData _oldPreset;
   int _row;
};

class RemoveRowCommand : public QUndoCommand
{
public:
   RemoveRowCommand( int row, LightPresetModel * owner) :
      QUndoCommand(),
      _row(row),
      _owner( owner)
   {
      T_ASSERT( _row >= 0);
      T_ASSERT( _row < _owner->m_presetDataStore.size());
      _preset = _owner->m_presetDataStore.at(_row);
   }

   // QUndoCommand interface
public:
   void undo() override
   {
      _owner->m_presetDataStore.insert( _row, _preset);
      _owner->layoutChanged();
   }

   void redo() override
   {
      _owner->m_presetDataStore.removeAt( _row);
      _owner->layoutChanged();
   }

private:
   int _row;
   LightPresetModel * _owner;
   LightPresetData _preset;
};


class MoveRowCommand : public QUndoCommand
{
public:
   MoveRowCommand( int rowFrom, int rowTo,
                   LightPresetModel * owner) :
      QUndoCommand(),
      _rowFrom(rowFrom),
      _rowTo(rowTo),
      _owner( owner)
   {
      T_ASSERT( (rowFrom >= 0) && (rowFrom < _owner->m_presetDataStore.size()));
      T_ASSERT( (rowTo >= 0) && (rowTo < _owner->m_presetDataStore.size()));
   }

public:
   void redo() override
   {
      _owner->m_presetDataStore.move( _rowFrom, _rowTo);
      _owner->layoutChanged();
   }

   void undo() override
   {
      _owner->m_presetDataStore.move( _rowTo, _rowFrom);
      _owner->layoutChanged();
   }

private:
   int _rowFrom, _rowTo;
   LightPresetModel * _owner;
};

}  // namespace LightPresetModelCommands


LightPresetModel::LightPresetModel(int numberOfChannels, LightPresetFactory *presetFactory,
                                   QObject *parent) :
   ExtAbstractListModel(parent),
   m_presetFactory(presetFactory),
   m_numberOfChannels(numberOfChannels),
   m_undoStack( new QUndoStack(this))
{
   T_ASSERT( m_presetFactory != nullptr);
   m_presetDataStore.clear();

   connect( m_undoStack, & QUndoStack::canRedoChanged, this, & LightPresetModel::canRedoChanged);
   connect( m_undoStack, & QUndoStack::canUndoChanged, this, & LightPresetModel::canUndoChanged);
   connect( m_undoStack, & QUndoStack::cleanChanged, this, & LightPresetModel::cleanChanged);
}

LightPresetModel::~LightPresetModel()
{
}

/**
 * @return retrive the number of stored preset
 */
int LightPresetModel::rowCount(const QModelIndex &) const
{
   return m_presetDataStore.length();
}

/**
 * @brief generic getter for an item of the model
 * @param index - identifies the desired preset
 * @param role - identifies what information is desired from preset at \p index
 * @return the desired info
 */
QVariant LightPresetModel::data(const QModelIndex &index, int role) const
{
   QVariant data = QVariant();

   int position = index.row();
   assertValidPosition( position);

   const LightPresetData & preset = m_presetDataStore.at(position);

   switch (role)
   {
   case modelViewRules::StringId:
   case Qt::DisplayRole:
      data = QVariant( preset.name() );
      break;
   case modelViewRules::Binary:
      data = QVariant::fromValue( preset );
      break;
   default:
      break;
   }

   return data;
}

bool LightPresetModel::setData(const QModelIndex & /*index*/, const QVariant & value, int role)
{
   bool res = false;

   if (role == modelViewRules::Binary)
   {
      const LightPresetData & lightData = value.value<LightPresetData>();
      updatePreset( lightData);
      res = true;

      layoutChanged();
   }

   return res;
}

bool LightPresetModel::removeRows(int row, int count, const QModelIndex &parent)
{
   T_ASSERT( row >= 0);
   T_ASSERT( (row + count) <= rowCount() );

   /* This operation is only supported in 2 circumstances: removing all
    * items or removing a single item. In the first case, undo stack is reset */
   if (count == rowCount())
   {
      m_presetDataStore.clear();
      m_undoStack->clear();
      emit layoutChanged();
   }
   else if (count == 1)
   {
      beginRemoveRows( parent, row, qMin(row + 1, m_presetDataStore.size()-1));
      LightPresetModelCommands::RemoveRowCommand * cmd =
            new LightPresetModelCommands::RemoveRowCommand( row, this);
      m_undoStack->push( cmd);
      endRemoveRows();
   }
   else
   {
      T_ASSERT_REP( false, "Light preset: move operation only allowed for single item or all items");
   }

   return true;
}

QVariant LightPresetModel::headerData(int /*section*/, Qt::Orientation /*orientation*/,
                                      int /*role*/) const
{
   return QVariant();
}


void LightPresetModel::createPresetWithValues(const QList<double> & values)
{
   updatePreset( newPresetName(), DEFAULT_TRNSITION_TIME, values);
}

/**
 * @brief add the preset passed as parameter in the model, if a
 *        preset with same name is not already present.
 *  If a preset with same 'name' is present, it is updated with
 *  passed \p preset parameters.
 * @param preset
 *
 * @note This function is equivalent to setData in canonical Model/View paqradigm
 */
void LightPresetModel::updatePreset(const LightPresetData &preset)
{
   /* search a preset with same name of passed one */
   QModelIndex indexOfExistingPreset = searchByName(preset.name());

   if (indexOfExistingPreset != QModelIndex())
   {
      /* update existing preset */
      LightPresetModelCommands::UpdateExistingPresetCommand * cmd =
            new LightPresetModelCommands::UpdateExistingPresetCommand( this, preset);
      m_undoStack->push( cmd);
   }
   else
   {
      /* add new preset */
      LightPresetModelCommands::AddNewPresetCommand * cmd =
            new LightPresetModelCommands::AddNewPresetCommand( this, m_presetDataStore.count(),preset);
      m_undoStack->push( cmd);
   }

   emit layoutChanged();
}

void LightPresetModel::updatePreset(const QString &name, int fadeMs, const QList<double> &targetValues)
{
   /* ensure there are enough values in preset list */
   QList<double> filledTargetValues = fillTargetValues( targetValues);

   LightPresetData *updatedItem = m_presetFactory->build( name, fadeMs, filledTargetValues, this);

   /* if preset is new, memory is correctly allocated. If it's an update
    * of an existing preset, function below will deallocate old item
    * and keep the newly created. */
   updatePreset( *updatedItem );
}

QList<double> LightPresetModel::fillTargetValues( const QList<double> &targetValues)
{
   QList<double> filledTargetValues = targetValues;

   if (filledTargetValues.count() < m_numberOfChannels)
   {
      for (int i=0; i < (m_numberOfChannels - targetValues.count()); i++)
      {
         filledTargetValues << 0.0;
      }
   }

   return filledTargetValues;
}

QString LightPresetModel::newPresetName() const
{
   /* return "DEFAULT_PRESET_NAME" unless it alredy exist.
    * In case, try to add a value.
    */
   int suffix = 1;
   QString presetName = QString("%1 %2").arg(DEFAULT_PRESET_NAME).arg( suffix);

   while (existsPresetWithName(presetName))
   {
      suffix++;
      presetName = QString("%1 %2").arg(DEFAULT_PRESET_NAME).arg( suffix);
   }

   return presetName;
}

bool LightPresetModel::existsPresetWithName( const QString & name) const
{
   bool exists = false;

   int totalEntries = m_presetDataStore.size();

   for (int i=0; (i < totalEntries) && ( ! exists); i++)
   {
      exists = ( m_presetDataStore[i].name() == name);
   }

   return exists;
}

/**
 * @brief if preset with name \p oldName is found, it is replaced with one
 *    with name \p newName. If \p oldName is not found, nothing is done.
 */
void LightPresetModel::renamePreset(const QString &oldName, const QString &newName)
{
   LightPresetModelCommands::RenamePresetCommand * cmd =
         new LightPresetModelCommands::RenamePresetCommand( this, oldName, newName);
   m_undoStack->push( cmd);
}

/**
 * @brief retrive model index for a preset, given the string of its name
 * @param targetName - name of desired preset
 * @return the model index, or QModelIndex() in case of no match
 */
QModelIndex LightPresetModel::searchByName(const QString &targetName) const
{
   int position = -1;
   bool found = false;

   for (int i = 0; (i < m_presetDataStore.length()) && (!found); i++)
   {
      if (m_presetDataStore.at(i).name() == targetName)
      {
         found = true;
         position = i;
      }
   }

   return index( position);
}

/**
 * @brief base class reimplementation. Source and destination indexes are ignored.
 * @param sourceRow - first row to be moved
 * @param count - number of rows moved from \p sourceRow
 * @param destinationChild - final position of the first row moved
 *
 * @return true if succesfully moved; false otherwise
 */
bool LightPresetModel::moveRows(const QModelIndex &/*sourceParent*/, int sourceRow, int count,
                                const QModelIndex &/*destinationParent*/, int destinationChild)
{
   T_ASSERT( count == 1);

   bool canMoveDown = (sourceRow + 1 == destinationChild) &&
                      (destinationChild < m_presetDataStore.count());
   bool canMoveUp = (destinationChild + 1 == sourceRow) &&
                    (sourceRow > 0);

   if (canMoveUp || canMoveDown)
   {
      LightPresetModelCommands::MoveRowCommand * cmd =
            new LightPresetModelCommands::MoveRowCommand( sourceRow, destinationChild, this);
      m_undoStack->push( cmd);
   }

   return (canMoveUp || canMoveDown);
}

void LightPresetModel::undo()
{
   m_undoStack->undo();
}

void LightPresetModel::redo()
{
   m_undoStack->redo();
}

void LightPresetModel::beginUndoMacro(const QString & name)
{
   m_undoStack->beginMacro( name);
}

void LightPresetModel::endUndoMacro()
{
   m_undoStack->endMacro();
}

void LightPresetModel::rearrangeItems(QList<QModelIndex> & selectedIndexes, int targetRow)
{
   /* a temporary list where items to be removed are placed */
   QList<LightPresetData> tempDataStore;

   /* extract indexes in reverse order to keep indexes valid */
   std::sort (selectedIndexes.begin(), selectedIndexes.end(), rowGreater);

   foreach( const QModelIndex & index, selectedIndexes)
   {
      T_ASSERT( index.row() != -1);
      tempDataStore.prepend( m_presetDataStore.at( index.row()));
      LightPresetModelCommands::RemoveRowCommand * removeCmd =
            new LightPresetModelCommands::RemoveRowCommand( index.row(), this);
      m_undoStack->push( removeCmd);

      /* if a removed item was before 'targetRow', then 'targetRow' decreases by one */
      if (index.row() < targetRow)
      {
         targetRow--;
      }
   }

   foreach (const LightPresetData & preset, tempDataStore)
   {
      LightPresetModelCommands::AddNewPresetCommand *appendCmd =
            new LightPresetModelCommands::AddNewPresetCommand( this, targetRow, preset);
      m_undoStack->push( appendCmd);

      ++targetRow;
   }
}


/**
 * @brief if \p position is outside model limits, an exception is rised
 * @param position - same as 'row' for model index
 */
void LightPresetModel::assertValidPosition(int position) const
{
   bool indexValid = (position >= 0);
   indexValid &= (position < m_presetDataStore.length());
   T_ASSERT_REP ( indexValid, QString("LPM: retriving data from invalid index: %1").arg(position));
}


Qt::ItemFlags LightPresetModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags = QAbstractListModel::flags(index);

   if (index.isValid())
   {
      flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable |
               Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
   }
   else
   {
      flags = Qt::ItemIsDropEnabled;
   }

   return  flags;
}

QStringList LightPresetModel::mimeTypes() const
{
   QStringList types;
   types << "text/uri-list"<<"lightPreset-data";
   return types;
}


QMimeData *LightPresetModel::mimeData(const QModelIndexList & indexes) const
{
   /* it seems that this is automagically deleted at the end of drag operation */
   QMimeData * mimeData = new QMimeData();

   QByteArray encodedData;
   QDataStream stream( & encodedData, QIODevice::WriteOnly);

   foreach (const QModelIndex &index, indexes)
   {
      if (index.isValid())
      {
         const LightPresetData & presetData = m_presetDataStore.at(index.row());
         stream << presetData.name();
      }
   }

   mimeData->setData( "lightPreset-data", encodedData);
   return mimeData;
}

