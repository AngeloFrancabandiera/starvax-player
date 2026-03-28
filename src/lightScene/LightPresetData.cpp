#include "LightPresetData.h"

/** nullptr object, to be used for invalid preset */
const LightPresetData NullPresetData;

LightPresetData::LightPresetData(int numOfChannels, QObject *parent) :
   QObject(parent),
   m_name(QString("Ambient")),
   m_fadeMs(100)
{
   m_levels = QList<double>();

   /* fill the list to set the size */
   for (int i = 0; i < numOfChannels; i++)
   {
      m_levels << 0;
   }
}

LightPresetData::LightPresetData(const QString &name, int fadeMS,
                                 const QList<double> &levelList, QObject *parent) :
   QObject(parent),
   m_name(name),
   m_fadeMs(fadeMS),
   m_levels(levelList)
{
}

LightPresetData::LightPresetData(const LightPresetData &other) :
   QObject(other.parent())
{
   m_name = other.name();
   m_fadeMs = other.fadeTime();
   m_levels = other.levelList();
}

/** @return true if the instance that called this method is the nullptr object */
bool LightPresetData::isNull() const
{
   return (this == &NullPresetData);
}

const LightPresetData &LightPresetData::Null()
{
   return NullPresetData;
}

LightPresetData &LightPresetData::operator =(const LightPresetData &other)
{
   m_name = other.m_name;
   m_fadeMs = other.m_fadeMs;
   m_levels = other.m_levels;
   return *this;
}

void LightPresetData::changeName(QString name )
{
   m_name = name;
}

void LightPresetData::changeFade(int fadeMs )
{
   m_fadeMs = fadeMs;
}

void LightPresetData::changeLevel(int channel, double value )
{
   if (channel < m_levels.length())
   {
      m_levels[channel] = value;
   }
}

void LightPresetData::setLevelList(const QList<double> &newValues)
{
   m_levels = newValues;
}

bool operator ==(const LightPresetData &left, const LightPresetData &right)
{
   return (left.name() == right.name()) &&
         (left.fadeTime() == left.fadeTime()) &&
         (left.levelList() == right.levelList());
}
