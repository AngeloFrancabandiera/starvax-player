#ifndef MEDIALISTMODEL_H
#define MEDIALISTMODEL_H

#include "ExtAbstractListModel.h"
#include "AbstractMediaSource.h"
#include "QDir"

class MediaSourceFactoryInterface;
class QIcon;
class QUndoStack;

namespace MediaModelCommands {
class AddNewItemCommand;
class SetDataCommand;
class RemoveRowCommand;
class MoveRowCommand;
} // namespace MediaModelCommands


class MediaListModel : public ExtAbstractListModel
{
   Q_OBJECT

   friend class MediaModelCommands::AddNewItemCommand;
   friend class MediaModelCommands::SetDataCommand;
   friend class MediaModelCommands::RemoveRowCommand;
   friend class MediaModelCommands::MoveRowCommand;
public:
   explicit MediaListModel( MediaSourceFactoryInterface *mediaFactory, QObject *parent = nullptr);
   ~MediaListModel() override;

   int rowCount(const QModelIndex &parent = QModelIndex()) const  override;

   QVariant data(const QModelIndex &index, int role) const  override;
   bool setData(const QModelIndex & index, const QVariant & value, int role)  override;
   Qt::ItemFlags flags(const QModelIndex &index) const  override;

   /** build a media entry from a \p fileName and insert it at the end of the list */
   void addMediaFile(const QString & fileName);                     /* !!! */
   const AbstractMediaSource *mediaData(const QModelIndex &index) const;
   QModelIndex indexForData(const AbstractMediaSource *data);

   bool insertRow(int row, const QModelIndex &parent = QModelIndex());
   bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
   bool moveRows(const QModelIndex &, int sourceRow, int count,
                 const QModelIndex &, int destinationChild) override;

   QModelIndex searchById( const QString & stringId) const;

   // a 'tag' can be added to the model to identify, for example, line A or B
   void setModelTag( const QString & modelTag) {
      m_modelTag = modelTag;
   }

   void undo() override;
   /** redo last operation on model  */
   void redo() override;
   void beginUndoMacro(const QString & name) override;
   void endUndoMacro() override;
   /** move a set of items (not necessarily contiuos) and store them
    *   continuosly at a given index
    * @param selectedIndexes holds the indexes that have been selected
    * @param targetRow is the row at which the first item will be placed after re-arrangement
    */
   void rearrangeItems( QList<QModelIndex> & selectedIndexes, int targetRow) override;

signals:

   void canRedoChanged(bool canRedo);
   void canUndoChanged(bool canUndo);
   void cleanChanged(bool clean);

public slots:
   void addMediaFiles( const QStringList & fileNames);
   /** undo last operation on model  */

private:
   QString createStringId( const QString &filename);
   bool setMediaData(const QModelIndex &index, const AbstractMediaSource *value);
   AbstractMediaSource::MediaKind mediaKindForFileName(const QString &fileName) const;

private:
   QList<const AbstractMediaSource *> m_mediaList;
   MediaSourceFactoryInterface *m_mediaFactory;
   QString m_modelTag;
   QUndoStack * m_undoStack;

   // QAbstractItemModel interface
public:
   virtual Qt::DropActions supportedDragActions() const  override;
   virtual QStringList mimeTypes() const  override;
   virtual QMimeData *mimeData(const QModelIndexList & indexes) const override;
};

#endif // MEDIALISTMODEL_H
