#ifndef SHOWFILEREPORTDIALOG_H
#define SHOWFILEREPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ShowFileReportDialog;
}

class ShowFileReportDialog : public QDialog
{
   Q_OBJECT

public:
   explicit ShowFileReportDialog(QWidget *parent = 0);
   ~ShowFileReportDialog();

   /** to be called before executing the Dialog */
   void setReportDetails( const QString & htmlText);

private slots:
   void on_okButton_clicked();

private:
   Ui::ShowFileReportDialog *ui;
};

#endif // SHOWFILEREPORTDIALOG_H
