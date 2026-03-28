#include "OpenWebNetPanel.h"
#include "ui_OpenWebNetPanel.h"
#include "OwnModel.h"
#include "OperativeOwnDelegate.h"
#include "EditModeOwnDelegate.h"
#include "ApplicationSettings.h"
#include "OwnFileFormatter.h"
#include "StatusDisplay.h"
#include "ApplicationIcon.h"

#include <QFile>
#include <QCommonStyle>
#include <QMessageBox>


OpenWebNetPanel::OpenWebNetPanel(QWidget *parent, OwnModel * model,
                                 OpenWebNetEngine_IF &ownEngine,
                                 ApplicationSettings & settings,
                                 StatusDisplay & display) :
   QWidget(parent),
   ui(new Ui::OpenWebNetPanel),
   m_model( model),
   m_settings( settings),
   m_display( display),
   m_modelChanged( false)
{
   ui->setupUi(this);

   ui->operativeModeTable->setModel( model);
   ui->editModeTable->setModel( model);

   ui->operativeModeTable->setColumnHidden( OwnModel::WHERE_COLUMN, true);
   ui->operativeModeTable->setColumnHidden( OwnModel::DIMMERABLE_COLUMN, true);

   QCommonStyle style;
   ui->addButton->setIcon( QIcon(IconPath("add.png")));
   ui->removeButton->setIcon( style.standardIcon( QStyle::SP_TrashIcon));
   ui->saveButton->setIcon( style.standardIcon( QStyle::SP_DialogOkButton));

   connect( ui->addButton, SIGNAL(released()),
            model, SLOT(addLightItem()));

   OperativeOwnDelegate * operativeDelegate = new OperativeOwnDelegate( ownEngine, this);
   ui->operativeModeTable->setItemDelegateForColumn( OwnModel::LABEL_COLUMN,
                                                     operativeDelegate);

   EditModeOwnDelegate * editDelegate = new EditModeOwnDelegate( this);
   ui->editModeTable->setItemDelegate( editDelegate);

   connect( model, SIGNAL(layoutChanged()), this, SLOT(onModelChanged()));

   ui->editModeTable->setSelectionMode(QAbstractItemView::SingleSelection);
   ui->editModeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->editModeTable->setDragEnabled(true);
}

OpenWebNetPanel::~OpenWebNetPanel()
{
   delete ui;
}

void OpenWebNetPanel::onConfigurationFileLoaded()
{
   ui->saveButton->hide();
   m_modelChanged = false;

   ui->operativeModeTable->resizeColumnsToContents();
}

void OpenWebNetPanel::setEditMode(bool editMode)
{
   ui->editModeTable->setVisible( editMode);
   ui->operativeModeTable->setVisible( ! editMode);

   ui->editGroupBox->setVisible( editMode);

   if ( ! editMode)
   {
      ui->operativeModeTable->setColumnWidth( OwnModel::LABEL_COLUMN, (this->width()*9)/10);
   }
   else
   {
      ui->operativeModeTable->resizeColumnsToContents();
   }
}

void OpenWebNetPanel::onModelChanged()
{
   ui->saveButton->show();
   m_modelChanged = true;
}



void OpenWebNetPanel::on_saveButton_clicked()
{
   /* retrieve currently used name. If empty, a default is returned */
   QString savePath = m_settings.getOpenWebNetSettings().configFilePath;

   /* format */
   OwnFileFormatter formatter( savePath);
   QString contents = formatter.format( m_model->openWebNetItems());

   /* save to file */
   QFile outFile( savePath);

   if (outFile.open( QIODevice::WriteOnly))
   {
      outFile.write( contents.toLatin1());
      outFile.close();

      m_display.showMessage( tr("updated file <i>%1</i>").arg(savePath),
                             StatusDisplay::INFORMATION);
   }
   else
   {
      m_display.showMessage( tr("Unable to open file <i>%1</i>").arg(savePath),
                             StatusDisplay::WARNING);
   }

   /* hide button when no modification must be saved */
   ui->saveButton->hide();
}

void OpenWebNetPanel::on_removeButton_clicked()
{
   /* sort indexes from highest to lowest and remove in this order.
    * In this way, when an element is removed, index of others does not change */
   QModelIndexList indexList = ui->editModeTable->selectionModel()->selectedRows();
   std::sort(indexList.begin(), indexList.end());

   /* remove one item per time, starting from last */
   for(int i = indexList.count() - 1; i >= 0; --i)
   {
      int row = indexList.at(i).row();

      /* remove row in model */
      m_model->removeRow( row, QModelIndex());
   }
}


void OpenWebNetPanel::checkToSave()
{
   /* detect if changes are in place with the presence of 'save' button */
   if (m_modelChanged)
   {
      int res = QMessageBox::question( this, tr("Open Web Net: save changes"),
                                       tr("Open Web Net configuration has changed.<br>"
                                          "Do you want to save changes?"));

      if (res == QMessageBox::Yes)
      {
         on_saveButton_clicked();
      }
   }
}

