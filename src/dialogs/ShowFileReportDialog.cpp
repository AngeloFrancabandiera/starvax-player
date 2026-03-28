#include "ShowFileReportDialog.h"
#include "ui_ShowFileReportDialog.h"

#include <QCommonStyle>

ShowFileReportDialog::ShowFileReportDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::ShowFileReportDialog)
{
   ui->setupUi(this);

   setWindowTitle( tr("Load show file"));

   /* information icon */
   QCommonStyle style;
   ui->iconLabel->setPixmap( QPixmap(style.standardIcon( QStyle::SP_MessageBoxInformation)
                             .pixmap(32,32)));
}

ShowFileReportDialog::~ShowFileReportDialog()
{
   delete ui;
}

void ShowFileReportDialog::setReportDetails(const QString & htmlText)
{
   ui->detailsEdit->setHtml( QString("<p><ul>%1</ul></p>").arg(htmlText));
}

void ShowFileReportDialog::on_okButton_clicked()
{
   done( QDialog::Accepted);
}
