#ifndef ACTIONTARGETSELECTORDIALOG_H
#define ACTIONTARGETSELECTORDIALOG_H

#include <QDialog>
#include "IF_ActionSelectorInterface.h"

class QAbstractListModel;
class QStringListModel;

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
   explicit ActionTargetSelectorDialog( QAbstractListModel *mediaModelLineA,
                                        QAbstractListModel * mediaModelLineB,
                                        QAbstractListModel *lightModel,
                                        QStringListModel& sequenceEntryModel,
                                        QWidget *parent = 0);
   ~ActionTargetSelectorDialog();

   /* IF_ActionSelectorInterface interface */
public:
   virtual void selectEvent(Type actionType);

private:
   Ui::ActionTargetSelectorDialog *ui;

private slots:
   void selectMediaPageLineA();
   void selectMediaPageLineB();
   void selectLightPage();
   void selectSequencerEntryPage();
   void selectPage(int page);
   void addSelectedItem();
   void on_addButton_clicked();
   void on_cancelButton_clicked();

private:
   void selectPageForEvent(Type actionType);
};

#endif // ACTIONTARGETSELECTORDIALOG_H
