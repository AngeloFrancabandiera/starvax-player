#ifndef EXPONENTIALFADER_H
#define EXPONENTIALFADER_H

#include "Fader.h"

class ExponentialFader : public Fader
{
   Q_OBJECT
public:
   explicit ExponentialFader(QObject *parent = nullptr);


private slots:
   void updateVolumeTickOut() override;
   void updateVolumeTickIn( int endVolume) override;

};

#endif // EXPONENTIALFADER_H
