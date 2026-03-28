#include "showfileparser.h"

#include <QFileDialog>
#include <QTextStream>
#include <QStringList>

#include "ShowFileStrings.h"
#include "supported_files.h"
#include "testableAssert.h"
#include "LightPresetData.h"


/** @file ShowFileParser.c
 *
 * This module takes an input stream and parse it to find show specific
 * tags ( such as playlist, light preset, ...) and store tags data to
 * local structures.
 */


/** constructor */
ShowFileParser::ShowFileParser(QTextStream &input, const QDir & basePath,
                               int numOfChannels, QObject *parent) :
   QObject(parent),
   m_activeMediaList( nullptr),
   m_basePath(basePath),
   m_numOfChannels(numOfChannels)
{
   T_ASSERT( input.device() != nullptr);

   m_source = new QTextStream(input.device());
   m_source->setEncoding( QStringConverter::Utf8);
}

ShowFileParser::~ShowFileParser()
{
   if (m_source != nullptr)
   {
      delete m_source;
   }
}

/** parse the whole stream and store elements in
  * internal data structure */
void ShowFileParser::parse()
{
   bool endOfTags = false;

   assertValidStream();

   m_source->seek(0);
   m_errorsLog.clear();
   m_sequencerScript.clear();

   while (!m_source->atEnd() && !endOfTags)
   {
      QString line = m_source->readLine();

      if (line == QString(PLAYLIST_LEGACY_START_TAG))
      {
         m_activeMediaList = & m_trackListLineA; /* By default, use line A */
         parseLinesUntilTag( QString(PLAYLIST_LEGACY_END_TAG), &ShowFileParser::readMediaLine );
         m_activeMediaList = nullptr;
      }
      else if (line == QString(PLAYLIST_LINE_A_START_TAG))
      {
         m_activeMediaList = & m_trackListLineA;
         parseLinesUntilTag( QString(PLAYLIST_LINE_A_END_TAG), &ShowFileParser::readMediaLine );
         m_activeMediaList = nullptr;
      }
      else if (line == QString(PLAYLIST_LINE_B_START_TAG))
      {
         m_activeMediaList = & m_trackListLineB;
         parseLinesUntilTag( QString(PLAYLIST_LINE_B_END_TAG), &ShowFileParser::readMediaLine );
         m_activeMediaList = nullptr;
      }
      else if (line == QString(LIGHTSET_START_TAG))
      {
         parseLinesUntilTag( QString(LIGHTSET_END_TAG), &ShowFileParser::readLightsetLine );
      }
      else if (line.startsWith(DEPRECATED_SEQUENCE_START_TAG))
      {
         parseLinesUntilTag( QString(DEPRECATED_SEQUENCE_END_TAG), &ShowFileParser::readSequenceLine );
      }
      else if (line.startsWith(SEQUENCER_START_TAG))
      {
         parseLinesUntilTag( QString(SEQUENCER_END_TAG), &ShowFileParser::readSequencerLine );
      }
      else if (line.startsWith(HTML_START_TAG))
      {
         /* once HTML starts, show tags are not parsed anymore */
         endOfTags = true;
      }
   }
}

void ShowFileParser::assertValidStream()
{
   T_ASSERT_REP (m_source->device() != nullptr,
                 QString("parsing invalid stream"));
}

/** this function is to be called when input stream has found
 *  an opening tag of a show feature. It reads all lines untill
 *  \p closingTag is found.
 *  If closing tag is not found, an error is logged.
 */
void ShowFileParser::parseLinesUntilTag(const QString &closingTag, LINE_PARSER_FUNCTION lineParser )
{
   bool endTagFound = false;
   bool htmlStartFound = false;

   while (!m_source->atEnd() && !endTagFound && !htmlStartFound)
   {
      QString line = m_source->readLine();

      if (line != closingTag)
      {
         if (line.startsWith(HTML_START_TAG))
         {
            /* closing tab should have already been found */
            htmlStartFound = true;
         }
         else
         {
            /* parse line */
            (this->*lineParser)( line );
         }
      }
      else
      {
         endTagFound = true;
      }
   }

   if (!endTagFound)
   {
      m_errorsLog << tr("End of features but %1 not found").arg(closingTag);
   }
}

/** each line in playlist tag is a relative path of a track file.
 *  That path is changed into absolute, if possible
 *  @par line - relative path of a media file. If \a m_basePath is valid,
 *     it's translated into an absolute path.
 */
void ShowFileParser::readMediaLine(const QString &line)
{
   T_ASSERT( m_activeMediaList);
   QString mediaPath = (m_basePath.exists()) ? m_basePath.absoluteFilePath( line ) : line;

   if (mediaFileIsValid( mediaPath))
   {
      *m_activeMediaList << mediaPath;
   }
   else
   {
      searchInAlternativePath( mediaPath);
   }
}

/**
 * check if media file can be added to project
 * @path line - full path of a media file
 * @return true if file exists and has valid extention
 */
bool ShowFileParser::mediaFileIsValid(const QString &path)
{
   bool fileIsValid = false;
   QString extention = path.split('.').last().toLower();

   if (QFileInfo::exists(path))
   {
      if (MEDIA_TRACKS_SUPPORTED_FORMATS.contains( extention ) ||
          PICTURE_SUPPORTED_FORMATS.contains( extention ))
      {
         fileIsValid = true;
      }
   }

   return fileIsValid;
}

void ShowFileParser::searchInAlternativePath( const QString & mediaPath)
{
   /* try to find the same base name in the list of alternative path.
    * If it's not found, ask user for another path */
   QFileInfo info = QFileInfo( mediaPath);
   QString basePath = info.absolutePath();
   QString name = info.fileName();

   bool found = false;

   foreach( QString path, m_alternativePathList)
   {
      QString alternateName = path + QDir::separator() + name;

      if (QFileInfo::exists(alternateName))
      {
         *m_activeMediaList << alternateName;
         logFileReplaced( mediaPath, alternateName);

         found = true;
         break;  // no other way to break cycle
      }
   }

   if ( ! found)
   {
      QString newFile;
      /* item can't be found. User must be asked for a new path */
      newFile = QFileDialog::getOpenFileName( nullptr, tr("Select an alternative location for %1").arg(name),
                                              basePath, MEDIA_TRACKS_DLG_FILTER);

      if (newFile != QString())
      {
         *m_activeMediaList << newFile;
         m_alternativePathList.prepend( QFileInfo(newFile).absolutePath());
         logFileReplaced( mediaPath, newFile);
      }
      else
      {
         logMissingFile(mediaPath);
      }
   }
}

void ShowFileParser::logFileReplaced( const QString& missingFile, QString newFile)
{
   m_errorsLog << tr("<li>Missing file <span style=\"color:DarkRed\"><i>%1</i></span><br>has been "
                     "replaced by <span style=\"color:DarkGreen\"><i>%2</i></li>")
                  .arg(missingFile,newFile);
}

void ShowFileParser::logMissingFile(const QString& missingFile)
{
   m_errorsLog << tr("<li>File not found or unsupported:"
                     " <span style=\"color:DarkRed\"><i>%1</i></li>").arg( missingFile);
}

void ShowFileParser::readLightsetLine(const QString &line)
{
   try
   {
      m_lightSetList << fillLightPresetFromLine( line);
   }
   catch(QString &msg)
   {
      /* parse error for this line. */
      m_errorsLog << QString( msg + ":\n" + line + "\n");
   }
}

/** parse input line @ref line and retrieve the data to
 *  fill into ligth preset @ref preset.
 *  Wrong syntax generates exceptions, but correctness of
 *  data respect to application settings is not checked.
 */
LightPresetData * ShowFileParser::fillLightPresetFromLine( const QString &line )
{
   QStringList lineFields, numberList;
   bool conversionOk, allConversionsOK = true;

   QList<double> levelSet;
   QString presetName;
   int fadeDuration;

   /* each line has: name (in double quotes), fade time, list of levels */
   lineFields = line.split(QChar('"'), STR_SPLIT::SkipEmptyParts);

   if (lineFields.length() < 2)
   {
      throw QObject::tr("Name or numeric values missing");
   }

   presetName = lineFields.at(0);

   numberList = lineFields.at(1).split(QChar(' '), STR_SPLIT::SkipEmptyParts);

   if (numberList.length() < 2)
   {
      throw QObject::tr("Not enough numbers");
   }

   /* first number is 'fade' time */
   fadeDuration = numberList.at(0).toInt(&conversionOk);
   allConversionsOK &= conversionOk;

   /* then comes level list */
   for (int i = 1; i < numberList.length(); i++)
   {
      levelSet << numberList[i].toDouble(&conversionOk);
      allConversionsOK &= conversionOk;
   }

   if (allConversionsOK == false)
   {
      throw QObject::tr("There are bad numerics");
   }

   /* pad channels with 0 if they are not enough */
   while (levelSet.size() < m_numOfChannels)
   {
      levelSet << 0.;
   }

   return new LightPresetData( presetName, fadeDuration, levelSet);
}

void ShowFileParser::readSequenceLine(const QString & /*line*/)
{
   m_errorsLog << tr("Tag </b>&lt;%1&gt;<b> is not supported anymore.\n"
                  "Please convert to new sequencer functionality. ").arg(DEPRECATED_SEQUENCE_START_TAG);
}

void ShowFileParser::readSequencerLine(const QString & line)
{
   m_sequencerScript.append(line).append('\n');
}


