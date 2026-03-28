#include "ActionTargetSelectorDialog.h"
#include "ui_ActionTargetSelectorDialog.h"

#include <QAbstractListModel>
#include <QFileInfo>
#include <QStringListModel>

#include "ApplicationIcon.h"
#include "testableAssert.h"
#include "supported_files.h"


#include "modelViewRules.h"

#define  MEDIA_LINE_A_TAB_INDEX    0   /**< tab position of media tab (must be coherent to UI) */
#define  MEDIA_LINE_B_TAB_INDEX    1   /**< tab position of media tab (must be coherent to UI) */
#define  LIGHT_TAB_INDEX    2   /**< tab position of light tab (must be coherent to UI) */
#define  SEQUENCER_ENTRY_TAB_INDEX    3   /**< tab position of sequencer (must be coherent to UI) */


ActionTargetSelectorDialog::ActionTargetSelectorDialog(QAbstractListModel *mediaModelLineA,
                                                        QAbstractListModel *mediaModelLineB,
                                                        QAbstractListModel *lightModel,
                                                        QStringListModel & sequenceEntryModel,
                                                        QWidget *parent) :
   QDialog(parent),
   ui(new Ui::ActionTargetSelectorDialog)
{
   ui->setupUi(this);

   connect( ui->eventTypeTab, & QTabWidget::currentChanged,
            this, & ActionTargetSelectorDialog::selectPage);
   selectMediaPageLineA();

   ui->lightList->setModel( lightModel);
   ui->mediaListLineA->setModel( mediaModelLineA);
   ui->mediaListLineB->setModel( mediaModelLineB);
   ui->sequenceEntryList->setModel( & sequenceEntryModel);

   connect( ui->lightList, & QListView::doubleClicked, this, & ActionTargetSelectorDialog::addSelectedItem);
   connect( ui->mediaListLineA, & QListView::doubleClicked, this, & ActionTargetSelectorDialog::addSelectedItem);
   connect( ui->mediaListLineB, & QListView::doubleClicked, this, & ActionTargetSelectorDialog::addSelectedItem);
   connect( ui->sequenceEntryList, & QListView::doubleClicked, this, & ActionTargetSelectorDialog::addSelectedItem);
}

ActionTargetSelectorDialog::~ActionTargetSelectorDialog()
{
   delete ui;
}

void ActionTargetSelectorDialog::selectEvent(Type actionType)
{
   selectPageForEvent( actionType);

   setModal( true);
   exec();
}

void ActionTargetSelectorDialog::selectPageForEvent(Type actionType)
{
   int pageNumber = -1;

   if ((actionType == MediaActionLineA) || (actionType == PictureActionLineA))
   {
      pageNumber = MEDIA_LINE_A_TAB_INDEX;
   }
   else if ((actionType == MediaActionLineB) || (actionType == PictureActionLineB))
   {
      pageNumber = MEDIA_LINE_B_TAB_INDEX;
   }
   else if (actionType == LightAction)
   {
      pageNumber = LIGHT_TAB_INDEX;
   }
   else if (actionType == SequenceEntryAction)
   {
      pageNumber = SEQUENCER_ENTRY_TAB_INDEX;
   }

   selectPage( pageNumber);
}

void ActionTargetSelectorDialog::selectPage(int page)
{
   switch (page)
   {
   case MEDIA_LINE_A_TAB_INDEX:
      selectMediaPageLineA();
      break;
   case MEDIA_LINE_B_TAB_INDEX:
      selectMediaPageLineB();
      break;
   case LIGHT_TAB_INDEX:
      selectLightPage();
      break;
   case SEQUENCER_ENTRY_TAB_INDEX:
      selectSequencerEntryPage();
      break;
   default:
      T_ASSERT(false);
      break;
   }
}


void ActionTargetSelectorDialog::selectMediaPageLineA()
{
   ui->eventTypeTab->setCurrentIndex( MEDIA_LINE_A_TAB_INDEX);
   ui->iconHolder->setPixmap( QPixmap(IconPath("sound_A.png")) );
   ui->lineLabel->setText(tr("LINE A"));
}

void ActionTargetSelectorDialog::selectMediaPageLineB()
{
   ui->eventTypeTab->setCurrentIndex( MEDIA_LINE_B_TAB_INDEX);
   ui->iconHolder->setPixmap( QPixmap(IconPath("sound_B.png")) );
   ui->lineLabel->setText(tr("LINE B"));
}

void ActionTargetSelectorDialog::selectLightPage()
{
   ui->eventTypeTab->setCurrentIndex( LIGHT_TAB_INDEX);
   ui->iconHolder->setPixmap( QPixmap(IconPath("light.png")) );
   ui->lineLabel->setText("");
}

void ActionTargetSelectorDialog::selectSequencerEntryPage()
{
   ui->eventTypeTab->setCurrentIndex( SEQUENCER_ENTRY_TAB_INDEX);
   ui->iconHolder->setPixmap( QPixmap(IconPath("sequencer_play.png")) );
   ui->lineLabel->setText("");
}

void ActionTargetSelectorDialog::on_addButton_clicked()
{
   addSelectedItem();
}

void ActionTargetSelectorDialog::addSelectedItem()
{
   QModelIndex selectedItem;

   if (ui->eventTypeTab->currentIndex() == MEDIA_LINE_A_TAB_INDEX)
   {
      m_actionType = IF_ActionSelectorInterface::MediaActionLineA;

      selectedItem = ui->mediaListLineA->currentIndex();
      m_actionId = selectedItem.data( modelViewRules::StringId).toString();

      /* picture is in same model as all other media
       * It can only be distinguished by file extention
       */
      QFileInfo actionInfo( m_actionId);

      if (PICTURE_SUPPORTED_FORMATS.contains( actionInfo.suffix(), Qt::CaseInsensitive))
      {
         m_actionType = IF_ActionSelectorInterface::PictureActionLineA;
      }
   }
   else if (ui->eventTypeTab->currentIndex() == MEDIA_LINE_B_TAB_INDEX)
   {
      m_actionType = IF_ActionSelectorInterface::MediaActionLineB;

      selectedItem = ui->mediaListLineB->currentIndex();
      m_actionId = selectedItem.data( modelViewRules::StringId).toString();

      /* picture is in same model as all other media
       * It can only be distinguished by file extenrtion
       */
      QFileInfo actionInfo( m_actionId);

      if (PICTURE_SUPPORTED_FORMATS.contains( actionInfo.suffix(), Qt::CaseInsensitive))
      {
         m_actionType = IF_ActionSelectorInterface::PictureActionLineB;
      }
   }
   else if (ui->eventTypeTab->currentIndex() == LIGHT_TAB_INDEX)
   {
      m_actionType = IF_ActionSelectorInterface::LightAction;

      selectedItem = ui->lightList->currentIndex();
      m_actionId = selectedItem.data( modelViewRules::StringId).toString();
   }
   else if (ui->eventTypeTab->currentIndex() == SEQUENCER_ENTRY_TAB_INDEX)
   {
      m_actionType = IF_ActionSelectorInterface::SequenceEntryAction;

      selectedItem = ui->sequenceEntryList->currentIndex();
      m_actionId = selectedItem.data().toString();
   }
   else
   {
      T_ASSERT(false);
   }

   done(QDialog::Accepted);
}

void ActionTargetSelectorDialog::on_cancelButton_clicked()
{
   m_actionType = IF_ActionSelectorInterface::NoAction;

   done(QDialog::Rejected);
}
