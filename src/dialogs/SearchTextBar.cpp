#include "SearchTextBar.h"
#include "ui_SearchTextBar.h"
#include <QKeyEvent>
#include <QTimer>

#include "ApplicationIcon.h"

namespace {
const uint32_t TEXT_NOT_FOUND_FLASH_DURATION_ms = 600u;
}

SearchTextBar::SearchTextBar(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::SearchTextBar),
   m_timer( new QTimer(this))
{
   ui->setupUi(this);

   ui->prevButton->setIcon( QIcon(IconPath("previos.png")) );
   ui->nextButton->setIcon( QIcon(IconPath("next.png")) );
   ui->quitButton->setIcon( QIcon(IconPath("close.png")) );

   connect( m_timer, SIGNAL(timeout()), this, SLOT(onTimeoutExpired()));
}

SearchTextBar::~SearchTextBar()
{
   delete ui;
}

void SearchTextBar::activateBar()
{
   setVisible( true);
   ui->lineEdit->setFocus();
   ui->lineEdit->selectAll();
}

void SearchTextBar::onTextNotFound()
{
   /* give a visual feedback of text not found */
   ui->lineEdit->setStyleSheet("background-color: red;");
   m_timer->start( TEXT_NOT_FOUND_FLASH_DURATION_ms);
}

void SearchTextBar::onTimeoutExpired()
{
   ui->lineEdit->setStyleSheet("");
   m_timer->stop();
}

void SearchTextBar::keyReleaseEvent(QKeyEvent * event)
{
   QWidget::keyReleaseEvent( event);

   /* enter or return key search forward; use shift to search backward */
   if ((event->key() == Qt::Key_Enter) ||
       (event->key() == Qt::Key_Return))
   {
      if (event->modifiers() & Qt::ShiftModifier)
      {
         emit searchTextBackward( ui->lineEdit->text());
      }
      else
      {
         emit searchTextForward( ui->lineEdit->text());
      }
   }
   else if ((event->key() == Qt::Key_Escape) &&
            (event->modifiers() & Qt::ShiftModifier))
   {
      hide();
   }
}


void SearchTextBar::on_quitButton_clicked()
{
   hide();
}

void SearchTextBar::on_prevButton_clicked()
{
   emit searchTextBackward( ui->lineEdit->text());
}

void SearchTextBar::on_nextButton_clicked()
{
   emit searchTextForward( ui->lineEdit->text());
}
