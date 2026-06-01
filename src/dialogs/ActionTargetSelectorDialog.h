#ifndef ACTIONTARGETSELECTORDIALOG_H
#define ACTIONTARGETSELECTORDIALOG_H

#include <array>
#include <QDialog>
#include <QModelIndex>
#include <map>
#include <vector>
#include "IF_ActionSelectorInterface.h"

class QAbstractListModel;
class QStringListModel;
class QModelIndex;
class QListView;


namespace Ui {
class ActionTargetSelectorDialog;
}

/**
 * A dialog that assists user in selecting a target for a script action.
 */
class ActionTargetSelectorDialog : public QDialog, public IF_ActionSelectorInterface
{
   Q_OBJECT

public:
   explicit ActionTargetSelectorDialog( std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & mediaModelSet,
                                        QAbstractListModel *lightModel,
                                        QStringListModel& sequenceEntryModel,
                                        QWidget *parent = 0);
   ~ActionTargetSelectorDialog();

   /* IF_ActionSelectorInterface interface */
public:
   virtual void selectEvent(Type actionType);

private:
   Ui::ActionTargetSelectorDialog *ui;
   QModelIndex m_currentlySelectedItem;
   std::map<const QAbstractListModel *, int> m_modelTags;
   std::vector<QListView *> m_playlistViewSet;

private slots:
   void selectMediaPage();
   void selectMediaSubPage(int deck_page);
   void selectLightPage();
   void selectSequencerEntryPage();
   void selectPage(int page);
   void selectItem(const QModelIndex &index);
   void addSelectedItem(const QModelIndex &index);
   void on_addButton_clicked();
   void on_cancelButton_clicked();

private:
   void selectPageForEvent(Type actionType);
   void deselect_all();
};

#endif // ACTIONTARGETSELECTORDIALOG_H
