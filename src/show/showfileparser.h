#ifndef SHOWFILEPARSER_H
#define SHOWFILEPARSER_H

#include <QObject>
#include <QDir>
#include <QList>
#include <array>


class QTextStream;
class LightPresetData;
class SequenceItem;

/** callback function that accepts one line parsed from a file */
class ShowFileParser;
typedef void (ShowFileParser::* LINE_PARSER_FUNCTION)(const QString &line);

class ShowFileParser : public QObject
{
   Q_OBJECT
public:
   explicit ShowFileParser(QTextStream &input, const QDir &basePath,
                           int numOfChannels, QObject *parent);
   ~ShowFileParser();

   void parse();

   /* getters */
   const QStringList & errorLog() const { return m_errorsLog; }
   QList<QString> trackListForDeck( int deck) const { return m_trackList[deck]; }
   QList<LightPresetData *> lightsetList() const { return m_lightSetList; }
   QList<SequenceItem *> sequenceItemList() const { return m_sequenceItemList; }
   const QString & sequencerScript() const  { return m_sequencerScript; }

private:
   QStringList m_errorsLog;
   std::array<QList<QString>, NUMBER_OF_MEDIA_DECKS> m_trackList;
   QList<QString> * m_activeMediaList; /** used to select one deck */
   QList<LightPresetData *> m_lightSetList;
   QList<SequenceItem *> m_sequenceItemList;
   QString m_sequencerScript;
   QTextStream *m_source;
   const QDir & m_basePath;
   int m_numOfChannels;
   QList<QString> m_alternativePathList;

private:
   void assertValidStream();
   void parseLinesUntilTag(const QString &closingTag , LINE_PARSER_FUNCTION lineParser);
   LightPresetData * fillLightPresetFromLine(const QString &line );
   bool mediaFileIsValid(const QString & path);
   void searchInAlternativePath( const QString & mediaPath);

private:    /* line parser functions */
   void readLightsetLine(const QString &line);
   void readMediaLine(const QString &line);
   void readSequenceLine(const QString &line);
   void readSequencerLine(const QString &line);
   void logFileReplaced(const QString& missingFile, QString newFile);
   void logMissingFile(const QString& missingFile);
};

#endif // SHOWFILEPARSER_H
