#ifndef LIGHTPRESETFACTORY_H
#define LIGHTPRESETFACTORY_H

#include <QList>
#include <QObject>

class QString;
class QObject;
class LightPresetData;

class LightPresetFactory : public QObject
{
public:
   LightPresetFactory( QObject *parent = nullptr)  : QObject(parent) {}

   LightPresetData *build( const QString & name, int fadeMS,
                           const QList<double> &levelList, QObject *parent = nullptr);
};

#endif // LIGHTPRESETFACTORY_H
