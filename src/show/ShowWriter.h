#ifndef SHOWWRITER_H
#define SHOWWRITER_H

#include <QObject>

class ShowFileFormatter;
class IF_ShowGuiInterface;

/** interface to write a show to persistent memory
 */
class IF_ShowWriterInterface : public QObject
{
   Q_OBJECT
public:
   IF_ShowWriterInterface( QObject *parent = 0) : QObject(parent) {}
   virtual ~IF_ShowWriterInterface() {}

   virtual void writeShow( IF_ShowGuiInterface &guiInterafce,
                           const QString & filePath) = 0;

   virtual QString savedFilePath() = 0;
};


/**
 * This class is in charge of writing the output of a
 * \a ShowFileFormatter class to a file, given its path.
 */
class ShowWriter : public IF_ShowWriterInterface
{
   Q_OBJECT
public:
   explicit ShowWriter( ShowFileFormatter *formatter, QObject *parent = 0);

   virtual void writeShow( IF_ShowGuiInterface &guiInterafce,
                           const QString & filePath);

   virtual QString savedFilePath() {
      return m_savedFilePath;
   }

private:
   ShowFileFormatter *m_formatter;
   QString m_savedFilePath;

private:
   void writeToFile( const QString & fileName, const QString & content);
};

#endif // SHOWWRITER_H
