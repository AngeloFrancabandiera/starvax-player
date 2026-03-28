#include "LightPresetPlaybar.h"
#include "ui_LightPresetPlaybar.h"
#include "ApplicationIcon.h"
#include "testableAssert.h"

#include <QMessageBox>
#include <QCommonStyle>

LightPresetPlaybar::LightPresetPlaybar(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::LightPresetPlaybar)
{
   ui->setupUi(this);
   m_levelView = ui->levelWidget;

   setContentsMargins( 2,2,2,2);
   layout()->setContentsMargins(2,2,2,2);

   T_ASSERT( m_levelView != nullptr);

   /* setup icons */
   QCommonStyle style;
   ui->playButton->setIcon( QIcon(IconPath("track_play.png")) );
   ui->deleteButton->setIcon( QIcon(IconPath("delete.png")) );
   ui->editButton->setIcon( QIcon(IconPath("edit.png")) );
   ui->updateButton->setIcon( QIcon(style.standardIcon( QStyle::SP_ArrowLeft)) );
}

LightPresetPlaybar::~LightPresetPlaybar()
{
   delete ui;
}


/**
 * @brief to be called to notify preset value changed. Used to draw level bars.
 * @param levels - list of light values in percent, range [0-100]
 */
void LightPresetPlaybar::setValueList(const QList<double> &levels)
{
   m_levelView->setLevelValues( levels);
}

void LightPresetPlaybar::setManualOnly(bool manualOnly, int index)
{
   m_levelView->setManualOnly( manualOnly, index);
}

void LightPresetPlaybar::setEditMode(bool editMode)
{
   ui->deleteButton->setVisible( editMode);
   ui->editButton->setVisible( editMode);
   ui->updateButton->setVisible( editMode);
}

void LightPresetPlaybar::on_playButton_clicked()
{
    emit play();
}

void LightPresetPlaybar::on_editButton_clicked()
{
    emit edit();
}

void LightPresetPlaybar::on_deleteButton_clicked()
{
   emit remove();
}

void LightPresetPlaybar::on_updateButton_clicked()
{
   emit updateFromSliders();
}
