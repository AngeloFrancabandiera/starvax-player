#include <QString>

#include "LightPresetFactory.h"
#include "LightPresetData.h"


LightPresetData *LightPresetFactory::build(const QString &name, int fadeMS,
                                           const QList<double> &levelList, QObject *parent)
{
   return new LightPresetData( name, fadeMS, levelList, parent);
}
