#ifndef LIGHTPRESETDATASTORE_H
#define LIGHTPRESETDATASTORE_H

#include <QObject>
#include <QString>
#include <QList>


class LightPresetData : public QObject
{
   Q_OBJECT
public:
   explicit LightPresetData(int numOfChannels, QObject *parent = nullptr);
   explicit LightPresetData(const QString & name, int fadeMS,
                            const QList<double> &levelList, QObject *parent = nullptr);

   /** nullptr constructor */
   LightPresetData() :
      m_name(QString("")),
      m_fadeMs(0)
   {}
   LightPresetData( const LightPresetData & other);
   ~LightPresetData() {}

   bool isNull() const;
   static const LightPresetData &Null();

   LightPresetData & operator =(const LightPresetData & other);

public:
   const QString &name() const { return m_name; }
   int fadeTime() const { return m_fadeMs; }
   const QList<double> &levelList() const { return m_levels; }

public slots:
   void changeName( QString name );
   void changeFade( int fadeMs );
   void changeLevel( int channel, double value );

   void setLevelList(const QList<double> & newValues);

private:
   QString m_name;
   int  m_fadeMs;
   QList<double>  m_levels;
};

Q_DECLARE_METATYPE(LightPresetData)

bool operator ==(const LightPresetData &left, const LightPresetData &right);

#endif // LIGHTPRESETDATASTORE_H
