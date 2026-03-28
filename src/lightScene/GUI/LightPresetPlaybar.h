#ifndef LIGHTPRESETPLAYBAR_H
#define LIGHTPRESETPLAYBAR_H

#include <QGroupBox>
#include <QList>

namespace Ui {
class LightPresetPlaybar;
}

class LevelViewForPreset;

class LightPresetPlaybar : public QWidget
{
   Q_OBJECT

public:
   explicit LightPresetPlaybar(QWidget *parent = nullptr);
   ~LightPresetPlaybar();

   void setValueList( const QList<double> &levels);

   /**
    * @brief draw one channel as manual only
    * @param manualOnly
    * @param index is zero based
    */
   void setManualOnly( bool manualOnly, int index);

signals:
   void play();
   void edit();
   void remove();
   /** request to update current preset from values in sliders */
   void updateFromSliders();

public slots:
   /** when \p editMode is true, GUI is allowed to add, move,
    *  change and delete presets */
   void setEditMode( bool editMode);

private slots:
   void on_playButton_clicked();
   void on_editButton_clicked();
   void on_deleteButton_clicked();
   void on_updateButton_clicked();

private:
   Ui::LightPresetPlaybar *ui;
   LevelViewForPreset *m_levelView;
};

#endif // LIGHTPRESETPLAYBAR_H
