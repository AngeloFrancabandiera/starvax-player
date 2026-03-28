#ifndef SEARCHTEXTBAR_H
#define SEARCHTEXTBAR_H

#include <QWidget>
class QTimer;

namespace Ui {
class SearchTextBar;
}

class SearchTextBar : public QWidget
{
   Q_OBJECT

public:
   explicit SearchTextBar(QWidget *parent = nullptr);
   ~SearchTextBar() override;

   /** make seach bar visible and set focus */
   void activateBar();

public slots:
   void onTextNotFound();

signals:
   void searchTextForward( const QString & text);
   void searchTextBackward( const QString & text);

private:
   Ui::SearchTextBar *ui;
   QTimer * m_timer;

   // QWidget interface
protected:
   void keyReleaseEvent(QKeyEvent * event) override;
private slots:
   void onTimeoutExpired();
   void on_quitButton_clicked();
   void on_prevButton_clicked();
   void on_nextButton_clicked();
};

#endif // SEARCHTEXTBAR_H
