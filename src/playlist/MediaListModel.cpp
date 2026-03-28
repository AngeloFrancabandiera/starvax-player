#include "MediaListModel.h"
#include <QStringList>
#include <QMimeData>
#include <QFileInfo>
#include <QUrl>
#include <QDataStream>

#include <QUndoStack>
#include <QUndoCommand>

#include "modelViewRules.h"
#include "MediaSourceFactory.h"
#include "testableAssert.h"
#include "supported_files.h"

#define INVALID_FILE    -1

/* insert element at the end of the list */
namespace { const int END_OF_LINE = -1; }

namespace MediaModelCommands
{
/**  */
class SetDataCommand : public QUndoCommand
{
public:
   SetDataCommand( int row, MediaListModel * model,
                   const QString & filePath) :
      QUndoCommand(),
      _row(row),
      _model( model)
   {
      AbstractMediaSource::MediaKind kind = model->mediaKindForFileName( filePath);
      _mediaSrc = model->m_mediaFactory->buildMediaSource(
                    filePath, model->createStringId(filePath), kind);
   }

   // QUndoCommand interface
public:
   void undo() override
   {
      int removeIndex = (_row == END_OF_LINE) ? (_model->m_mediaList.count() - 1) : _row;
      _model->m_mediaList.removeAt( removeIndex);
      _model->layoutChanged();
   }

   void redo() override
   {
      int insertIndex = (_row == END_OF_LINE) ? _model->m_mediaList.count() : _row;

      _model->insertRow( insertIndex );
      _model->setMediaData( _model->index( insertIndex, 0), _mediaSrc );
      _model->layoutChanged();
   }

private:
   int _row;
   MediaListModel * _model;
   AbstractMediaSource * _mediaSrc;
};

/** remove a row in model at given position. */
class RemoveRowCommand : public QUndoCommand
{
public:
   RemoveRowCommand( int row, MediaListModel * model) :
      QUndoCommand(),
      _row(row),
      _model( model)
   {
      //filePath = _model->mediaData(_model->index(_row))->fileName();
      _mediaSrc = _model->mediaData(_model->index(_row));
   }

   // QUndoCommand interface
public:
   void undo() override
   {
      _model->insertRow( _row );
      _model->setMediaData( _model->index( _row), _mediaSrc );
      _model->layoutChanged();
   }

   void redo() override
   {
      _model->m_mediaList.removeAt( _row);
      _model->layoutChanged();
   }

private:
   int _row;
   MediaListModel * _model;
   //QString filePath;
   const AbstractMediaSource * _mediaSrc;
};

/**  */
class MoveRowCommand : public QUndoCommand
{
public:
   MoveRowCommand( int rowFrom, int rowTo,
                   MediaListModel * owner) :
      QUndoCommand(),
      _rowFrom(rowFrom),
      _rowTo(rowTo),
      _owner( owner)
   {
      T_ASSERT( (rowFrom >= 0) && (rowFrom < _owner->m_mediaList.size()));
      T_ASSERT( (rowTo >= 0) && (rowTo < _owner->m_mediaList.size()));
   }

public:
   void redo() override
   {
      _owner->m_mediaList.move( _rowFrom, _rowTo);
      _owner->layoutChanged();
   }

   void undo() override
   {
      _owner->m_mediaList.move( _rowTo, _rowFrom);
      _owner->layoutChanged();
   }

private:
   int _rowFrom, _rowTo;
   MediaListModel * _owner;
};

/**  */
class AddNewItemCommand : public QUndoCommand
{
public:
   AddNewItemCommand( MediaListModel * owner,
                      int position,
                      const AbstractMediaSource * item ) :
      _owner( owner),
      _position( position),
      _newItem( item)
   {
   }

   // QUndoCommand interface
public:
   void redo() override
   {
      _owner->m_mediaList.insert( _position, _newItem);
      emit _owner->layoutChanged();
   }

   void undo() override
   {
      if (_owner->m_mediaList.size() > 0)
      {
         _owner->m_mediaList.removeAt( _position);
         emit _owner->layoutChanged();
      }
   }

private:
   MediaListModel * _owner;
   int _position;
    const AbstractMediaSource * _newItem;
};

}  // namespace MediaModelCommands

MediaListModel::MediaListModel(MediaSourceFactoryInterface *mediaFactory,
                               QObject *parent) :
   ExtAbstractListModel(parent),
   m_mediaFactory(mediaFactory),
   m_undoStack( new QUndoStack(this))
{
   T_ASSERT(mediaFactory != nullptr);

   connect( m_undoStack, & QUndoStack::canRedoChanged, this, & MediaListModel::canRedoChanged);
   connect( m_undoStack, & QUndoStack::canUndoChanged, this, & MediaListModel::canUndoChanged);
   connect( m_undoStack, & QUndoStack::cleanChanged, this, & MediaListModel::cleanChanged);
}

MediaListModel::~MediaListModel()
{
}


int MediaListModel::rowCount(const QModelIndex &/*parent*/) const
{
   return m_mediaList.count();
}


const AbstractMediaSource *MediaListModel::mediaData(const QModelIndex &index) const
{
   const AbstractMediaSource *data = nullptr;

   // chek if params are valid
   T_ASSERT_REP( (index.isValid()) && (index.row() < m_mediaList.count()),
                 QString("MediaListModel::mediaData: invalid index %1").arg(index.row()));

   data = m_mediaList.at( index.row() );

   return data;
}


QVariant MediaListModel::data(const QModelIndex &index, int role) const
{
   QVariant data = QVariant();

   /* chek if params are valid */
   if ((index.isValid()) && (index.row() < m_mediaList.count()))
   {
      int row = index.row();
      const AbstractMediaSource * mediaSource = m_mediaList.at(row);
      T_ASSERT( mediaSource != nullptr);

      switch (role)
      {
      case Qt::DisplayRole:
      case modelViewRules::StringId:
         data = QVariant( m_mediaList.at(row)->stringId() );
         break;
      case Qt::ToolTipRole:
         /* show complete file path */
         data = QVariant( mediaSource->fileName() );
         break;

      case modelViewRules::Binary:
         data = QVariant::fromValue<const AbstractMediaSource *>( mediaSource);
         break;

      case modelViewRules::Serialize:
         data = QVariant( mediaSource->fileName() );
         break;

      default:
         break;
      }
   }

   return data;
}

bool MediaListModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
   bool valid = false;

   if (role == modelViewRules::CreateFromString)
   {
      T_ASSERT( index.row() < rowCount());
      QString filePath = value.toString();

      /* index -1 for 'end of list' */
      int insertIndex = (index.row() == -1) ? m_mediaList.count() : index.row();

      MediaModelCommands::SetDataCommand * cmd =
            new MediaModelCommands::SetDataCommand( insertIndex, this, filePath);
      m_undoStack->push( cmd);

      valid = true;
   }

   return valid;
}

bool MediaListModel::setMediaData(const QModelIndex &index, const AbstractMediaSource *value)
{
   bool done = false;

   if ((index.row() < m_mediaList.count()) && (index.row() >= 0))
   {
      // store value in list
      m_mediaList[index.row()] = value;

      // notify views
      emit dataChanged( index, index);

      done = true;
   }

   return done;
}


AbstractMediaSource::MediaKind
MediaListModel::mediaKindForFileName(const QString & fileName) const
{
   AbstractMediaSource::MediaKind kind = AbstractMediaSource::KIND_NONE;
   QString suffix = QFileInfo(fileName).suffix();

   if (PICTURE_SUPPORTED_FORMATS.contains( suffix))
   {
      if (m_modelTag == "PB")
      {
         kind = AbstractMediaSource::PICTURE_LINE_B;
      }
      else  // "PA" and default
      {
         kind = AbstractMediaSource::PICTURE_LINE_A;
      }
   }
   else if (MEDIA_TRACKS_SUPPORTED_FORMATS.contains( suffix))
   {
      if (m_modelTag == "PB")
      {
         kind = AbstractMediaSource::AUDIO_VIDEO_LINE_B;
      }
      else  // "PA" and default
      {
         kind = AbstractMediaSource::AUDIO_VIDEO_LINE_A;
      }
   }

   return kind;
}

Qt::ItemFlags MediaListModel::flags(const QModelIndex &index) const
{
   Qt::ItemFlags flags = Qt::NoItemFlags;

   if (index.isValid())
   {
      flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable |
              Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
   }
   else
   {
      flags = Qt::ItemIsDropEnabled;
   }

   return  flags;
}

/**
 * normally a media file is identified by the base name of file (with
 * extention). In case of conflict, a tail with a sequence number is added.
 * Confilcts may be due to media in different folder with same name or to using
 * the same media more than once.
 * @param full path of media file
 * @return media unique identifier
 */
QString MediaListModel::createStringId(const QString &filename)
{
   QString baseName = QFileInfo(filename).fileName();
   int baseNameUsage = 0;

   QString stringId = baseName; /* default case */

   foreach (const AbstractMediaSource *source, m_mediaList )
   {
      if (source->stringId().startsWith(baseName))
      {
         baseNameUsage++;
      }
   }

   if (baseNameUsage > 0)
   {
      stringId.append( QString(" (%1)").arg( baseNameUsage + 1 ) );
   }

   return stringId;
}

void MediaListModel::addMediaFiles(const QStringList &fileNames)
{
   foreach( QString file, fileNames)
   {
      addMediaFile( file);
   }
}

void MediaListModel::undo()
{
   m_undoStack->undo();
}

void MediaListModel::redo()
{
   m_undoStack->redo();
}


void MediaListModel::addMediaFile( const QString &fileName)
{
   MediaModelCommands::SetDataCommand * cmd =
         new MediaModelCommands::SetDataCommand( rowCount(), this, fileName);
   m_undoStack->push( cmd);
}


QModelIndex MediaListModel::indexForData(const AbstractMediaSource *data)
{
   QModelIndex index;

   int position = m_mediaList.indexOf( data );

   if (position == -1)
   {
      // item not found
      index = QModelIndex();
   }
   else
   {
      index = this->index( position );
   }

   return index;
}



/**  insert a row in the model in the given position */
bool MediaListModel::insertRow(int row, const QModelIndex &/*parent*/)
{
   bool ret = false;

   if ((row >= 0) && (row <=  m_mediaList.count()))
   {
      // insert a dummy source
      m_mediaList.insert( row, nullptr );
      emit layoutChanged();

      ret = true;
   }

   return ret;
}


bool MediaListModel::removeRows(int row, int count,
                                const QModelIndex &parent )
{
   T_ASSERT( row >= 0);
   T_ASSERT( (row + count) <= rowCount() );

   /* This operation is only supported in 2 circumstances: removing all
    * items or removing a single item. In the first case, undo stack is reset */
   if (count == rowCount())
   {
      m_mediaList.clear();
      m_undoStack->clear();
      emit layoutChanged();
   }
   else if (count == 1)
   {
      beginRemoveRows( parent, row,  qMin(row + 1, m_mediaList.size()-1));
      MediaModelCommands::RemoveRowCommand * cmd =
            new MediaModelCommands::RemoveRowCommand( row, this);
      m_undoStack->push( cmd);
      endRemoveRows();
   }
   else
   {
      T_ASSERT_REP( false, "Media: move operation only allowed for single item or all items");
   }

   return true;
}


QModelIndex MediaListModel::searchById(const QString &stringId) const
{
   QModelIndex target = QModelIndex();

   for (int i=0; (i < m_mediaList.length()) && (target == QModelIndex()); i++)
   {
      if (m_mediaList.at(i)->stringId() == stringId)
      {
         target = index(i);
      }
   }

   return target;
}

/**
 * @brief base class reimplementation. Source and destination indexes are ignored.
 * @param sourceRow - first row to be moved
 * @param count - number of rows moved from \p sourceRow
 * @param destinationChild - final position of the first row moved
 *
 * @return true if succesfully moved; false otherwise
 */
bool MediaListModel::moveRows(const QModelIndex &/*sourceParent*/, int sourceRow, int count,
                                const QModelIndex &/*destinationParent*/, int destinationChild)
{
   T_ASSERT( count == 1);

   bool canMoveDown = (sourceRow + 1 == destinationChild) &&
                      (destinationChild < m_mediaList.count());
   bool canMoveUp = (destinationChild + 1 == sourceRow) &&
                    (sourceRow > 0);

   if (canMoveUp || canMoveDown)
   {
      MediaModelCommands::MoveRowCommand * cmd =
            new MediaModelCommands::MoveRowCommand( sourceRow, destinationChild, this);
      m_undoStack->push( cmd);
   }

   return (canMoveUp || canMoveDown);
}


Qt::DropActions MediaListModel::supportedDragActions() const
{
   return Qt::CopyAction | Qt::MoveAction;
}


QStringList MediaListModel::mimeTypes() const
{
   QStringList types;
   types <<"playlist-data";
   return types;
}


QMimeData *MediaListModel::mimeData(const QModelIndexList & indexes) const
{
   /* it seems that this is automagically deleted at the end of drag operation */
   QMimeData *mimeData = new QMimeData();

   QByteArray encodedData;
   QDataStream stream( & encodedData, QIODevice::WriteOnly);
   const AbstractMediaSource * mediaSource = nullptr;

   foreach (const QModelIndex &index, indexes)
   {
      if (index.isValid())
      {
         mediaSource = m_mediaList.at(index.row());
         T_ASSERT( mediaSource != nullptr);

         // serialize all fields of 'mediaSource'
         stream << static_cast<int>(mediaSource->mediaKind());
         stream << mediaSource->stringId();
         stream << mediaSource->fileName();
      }
   }

   mimeData->setData( "playlist-data", encodedData);

   return mimeData;
}


void MediaListModel::beginUndoMacro(const QString & name)
{
   m_undoStack->beginMacro( name);
}

void MediaListModel::endUndoMacro()
{
   m_undoStack->endMacro();
}


void MediaListModel::rearrangeItems(QList<QModelIndex> & selectedIndexes, int targetRow)
{
   /* a temporary list where items to be removed are placed */
   QList<const AbstractMediaSource *> tempDataStore;

   /* extract indexes in reverse order to keep indexes valid */
   std::sort (selectedIndexes.begin(), selectedIndexes.end(), rowGreater);

   foreach( const QModelIndex & index, selectedIndexes)
   {
      T_ASSERT( index.row() != -1);
      tempDataStore.prepend( m_mediaList.at( index.row()));
      MediaModelCommands::RemoveRowCommand * removeCmd =
            new MediaModelCommands::RemoveRowCommand( index.row(), this);
      m_undoStack->push( removeCmd);

      /* if a removed item was before 'targetRow', then 'targetRow' decreases by one */
      if (index.row() < targetRow)
      {
         targetRow--;
      }
   }

   foreach (const AbstractMediaSource * media, tempDataStore)
   {
      MediaModelCommands::AddNewItemCommand *addCmd =
            new MediaModelCommands::AddNewItemCommand( this, targetRow, media);
      m_undoStack->push( addCmd);

      ++targetRow;
   }
}
