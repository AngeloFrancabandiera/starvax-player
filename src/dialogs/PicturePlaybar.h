#ifndef PICTUREPLAYBAR_H
#define PICTUREPLAYBAR_H

#include <QWidget>
#include "IF_MediaEngineInterface.h"

namespace Ui {
class PicturePlaybar;
}

class QAbstractScrollArea;
class AbstractMediaSource;
class MediaAutomation;


class PicturePlaybar : public QWidget
{
   Q_OBJECT

public:
   explicit PicturePlaybar(IF_MediaEngineInterface * engine, MediaAutomation &automation,
                           QWidget *parent = nullptr);
   ~PicturePlaybar() override;

   /**
    * @brief set the Widget on which a thumbnail of current picture will
    *   be displayed in upper right corner
    * @param container
    */
   void setThumbnailContainer( QAbstractScrollArea * container);

public slots:
   void showPicture();
   void onPictureShowChanged( bool isShown);

   // QWidget interface
protected:
   virtual void paintEvent(QPaintEvent * event) override;

private slots:
   void onCurrentMediaChanged(const AbstractMediaSource & picture);


private:
   Ui::PicturePlaybar *ui;
   IF_MediaEngineInterface * m_engine;
   QAbstractScrollArea * m_thumbnailContainer;

};

#endif // PICTUREPLAYBAR_H
