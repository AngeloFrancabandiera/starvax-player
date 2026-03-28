#include "ShowFileFactory.h"

#include "showfileparser.h"

ShowFileFactory::ShowFileFactory(int numOfLightChannels, QObject *parent) :
   QObject(parent),
   m_numOfLightChannels(numOfLightChannels)
{
}

ShowFileParser *ShowFileFactory::buildShowParser( QTextStream & source, const QDir & folder)
{
   return new ShowFileParser( source, folder, m_numOfLightChannels, this);
}

