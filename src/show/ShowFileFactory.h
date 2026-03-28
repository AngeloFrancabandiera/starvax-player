#ifndef SHOWFILEFACTORY_H
#define SHOWFILEFACTORY_H

#include <QObject>

class QTextStream;
class QDir;

class ShowFileParser;

/**
 * This factory builds object that operate on a show file to be
 * read or written, when user has established the name of the file
 */
class ShowFileFactory : public QObject
{
   Q_OBJECT
public:
   explicit ShowFileFactory(int numOfLightChannels, QObject *parent = 0);

   ShowFileParser *buildShowParser(QTextStream & source, const QDir & folder);

private:
   int m_numOfLightChannels;
};

#endif // SHOWFILEFACTORY_H
