#ifndef LIGHTSETEDITOR_H
#define LIGHTSETEDITOR_H

#include <QGroupBox>
#include "QList"

class QDoubleSpinBox;

namespace Ui {
class LightSetEditor;
}

class LightSetEditor : public QGroupBox
{
   Q_OBJECT

public:
   explicit LightSetEditor( int numberOfChannels, QWidget *parent = nullptr);
   ~LightSetEditor();

   QString name();
   void setName(const QString &name);

   int fadeMs();
   void setFadeMs( int fadeMs);

   QList<double> levelList();
   void setLevelList( const QList<double> &levelList);

signals:
   void editComplete( LightSetEditor * editor);

private slots:
   void on_doneButton_clicked();

private:
   Ui::LightSetEditor *ui;
   QList<QDoubleSpinBox *> m_levelSpinList;
};

#endif // LIGHTSETEDITOR_H
