#include "Sequencer_LineParser.h"

//#include <QStringView>
#include <QStringList>
#include <QMap>
#include <QRegularExpression>

namespace {

/* character used to begin a line comment */
const char COMMENT_CHAR = '#';
const QString FOREVER_STRING = QString::fromLatin1("forever");

/* a reentrant function that parses a line whose kind is well known */
typedef bool (Sequencer::LineParser::* LINE_PARSER_FUNCTION)(const QString &line);

QMap<QString, Sequencer::LineKind> MainKeywordTable;
QMap<QString, QRegularExpression *> KeywordRegExpTable;
QMap<QString, LINE_PARSER_FUNCTION> LineParseTable;
QMap<QString, Sequencer::PlaylistSubCommand> PlaylistSubcommandTable;
QMap<QString, Sequencer::LightSubCommand> LightSubcommandTable;
QMap<QString, Sequencer::SequenceSubCommand> SequenceSubcommandTable;
QMap<QString, Sequencer::HallSubCommand> HallSubcommandTable;

}


namespace Sequencer {

LineParser::LineParser() :
   m_lineKind( LINE_BLANK)
{
   /* fill lookup tables */
   MainKeywordTable.insert( "repeat",   LINE_REPEAT);
   MainKeywordTable.insert( "end",      LINE_END);
   MainKeywordTable.insert( "playlist", LINE_PLAYLIST);
   MainKeywordTable.insert( "light",    LINE_LIGHT);
   MainKeywordTable.insert( "sequence", LINE_SEQUENCE);
   MainKeywordTable.insert( "hall",     LINE_HALL);
   MainKeywordTable.insert( "entry",    LINE_ENTRY);

   KeywordRegExpTable.insert( "repeat",   new QRegularExpression("\\s*repeat\\b\\s*((\\d+)|forever)+\\s*$"));
   KeywordRegExpTable.insert( "end",      new QRegularExpression("\\s*end\\b\\s*$"));
   KeywordRegExpTable.insert( "playlist",
                              new QRegularExpression("\\s*playlist\\b\\s*(?<line>A|B)\\s+(?<cmd>\\w+)\\s*(\"(?<media>.*)\")?(?<numVal>\\d+)?\\s*$",
                                                     QRegularExpression::DontCaptureOption));

   KeywordRegExpTable.insert( "light",
                              new QRegularExpression("\\s*light\\b\\s+(?<cmd>\\w+)\\s*(\"(?<preset>.*)\")?\\s*$",
                                                     QRegularExpression::DontCaptureOption));
   KeywordRegExpTable.insert( "sequence",
                              new QRegularExpression("\\s*sequence\\b\\s+(?<cmd>\\w+)\\s*(?<time>\\d+(\\.\\d+)?)?\\s*$",
                                                     QRegularExpression::DontCaptureOption));
   KeywordRegExpTable.insert( "hall",
                              new QRegularExpression("\\s*hall\\b\\s*(?<cmd>\\w+)\\s*(\"(?<zone>.+)\")?\\s*(?<level>\\d+)?\\s*$",
                                                     QRegularExpression::DontCaptureOption));
   KeywordRegExpTable.insert( "entry",
                              new QRegularExpression("\\s*entry\\b\\s*\"(?<label>[^\"]+)\"\\s*:\\s*$",
                                                     QRegularExpression::DontCaptureOption));

   LineParseTable.insert( "repeat",   & LineParser::parseRepeat);
   LineParseTable.insert( "end",      & LineParser::parseEnd);
   LineParseTable.insert( "playlist", & LineParser::parsePlaylist);
   LineParseTable.insert( "light",    & LineParser::parseLight);
   LineParseTable.insert( "sequence", & LineParser::parseSequence);
   LineParseTable.insert( "hall",     & LineParser::parseHall);
   LineParseTable.insert( "entry",    & LineParser::parseEntry);

   PlaylistSubcommandTable.insert("play", PLAYLIST_PLAY);
   PlaylistSubcommandTable.insert("playAll", PLAYLIST_PLAY_ALL);
   PlaylistSubcommandTable.insert("pause", PLAYLIST_PAUSE);
   PlaylistSubcommandTable.insert("stop", PLAYLIST_STOP);
   PlaylistSubcommandTable.insert("show", PLAYLIST_SHOW);
   PlaylistSubcommandTable.insert("hide", PLAYLIST_HIDE);
   PlaylistSubcommandTable.insert("top", PLAYLIST_SET_TOP);
   PlaylistSubcommandTable.insert("bottom", PLAYLIST_SET_BOTTOM);
   PlaylistSubcommandTable.insert("rewind", PLAYLIST_REWIND);
   PlaylistSubcommandTable.insert("fadeout", PLAYLIST_FADEOUT);
   PlaylistSubcommandTable.insert("volume", PLAYLIST_VOLUME);
   PlaylistSubcommandTable.insert("wait", PLAYLIST_WAIT);

   LightSubcommandTable.insert( "trigger", LIGHT_TRIGGER);
   LightSubcommandTable.insert( "stop", LIGHT_STOP);

   SequenceSubcommandTable.insert( "pause", SEQUENCE_PAUSE);
   SequenceSubcommandTable.insert( "stop", SEQUENCE_STOP);

   HallSubcommandTable.insert("on", HALL_ON);
   HallSubcommandTable.insert("off", HALL_OFF);

}

LineParser::~LineParser()
{

}

bool LineParser::parseLine(const QString &line)
{
   bool validCommand = false;  // flag for main command found
   bool found = false;

   QStringView stringRef( line);

   stringRef = stringRef.trimmed();

   /* preset output state */
   m_lineKind = LINE_BLANK;
   m_errorString.clear();

   if ((stringRef.size() > 0) &&
       (stringRef.at(0) != COMMENT_CHAR))
   {
      QMap<QString, LineKind>::const_iterator i = MainKeywordTable.constBegin();

      while ((i != MainKeywordTable.constEnd()) &&
             ( ! found))
      {
         if (stringRef.startsWith(i.key()))
         {
            m_lineKind = i.value();
            found = true;

            /* invoke relevant parser */
            validCommand = (this->*LineParseTable.value( i.key()))( line );
         }
         else
         {
            ++i;
         }
      }

      if ( ! found)
      {
         m_lineKind = LINE_INVALID;
         m_errorString = QString::fromLatin1("invalid primary command");
      }
   }

   return validCommand;
}


LineKind LineParser::getLineKind() const
{
   return m_lineKind;
}

bool LineParser::parseRepeat( const QString & line)
{
   bool res = false;
   QRegularExpression * regexp = KeywordRegExpTable.value( "repeat");
   QRegularExpressionMatch match = regexp->match( line);

   if (match.hasMatch())
   {
      /* check wether param is a number */
      m_lineParamsRepeat.times = match.captured(1).toInt( & res);

      /* if not, it may be 'forever' */
      if ((res == false) && match.captured(1) == FOREVER_STRING)
      {
         res = true;
         m_lineParamsRepeat.times = REPEAT_FOREVER;
      }
   }

   if (res == false)
   {
      m_lineKind = LINE_INVALID;
      m_errorString = QObject::tr("<b>syntax error.</b>. "
                                  "<p>Usage: <b>repeat <i>N</i></b></p>"
                                  "<p> <i>N</i> is an integer number. It can be <b>forever</b>"
                                  " to indicate an infinite loop</p>");
   }

   return res;
}

bool LineParser::parseEnd( const QString & line)
{
   bool res = false;
   QRegularExpression * regexp = KeywordRegExpTable.value( "end");
   QRegularExpressionMatch match = regexp->match( line);

   if (match.hasMatch())
   {
      /* 'end' has no parameters */
      res = true;
   }
   else
   {
      m_lineKind = LINE_INVALID;
      m_errorString = QObject::tr("<b>syntax error.</b>. "
                                  "Usage: <b>end</b>");
   }

   return res;
}


bool LineParser::parsePlaylist( const QString & line)
{
   bool res = false;
   QRegularExpression * regexp = KeywordRegExpTable.value( "playlist");
   QRegularExpressionMatch match = regexp->match( line);

   QString errorDetail;

   if (match.hasMatch())
   {
      /* check parsed values */
      QChar playlistLine = match.captured("line").at(0);

      m_lineParamsPlaylist.line = (playlistLine == 'A') ? Playlist::LINE_A :Playlist::LINE_B;
      m_lineParamsPlaylist.subCommand = PlaylistSubcommandTable.value( match.captured("cmd"),
                                                                       PLAYLIST_NONE);

      if (m_lineParamsPlaylist.subCommand != PLAYLIST_NONE)
      {
         bool convOk = false;
         /* just store the label. Existence of media will be checked by media engine */
         m_lineParamsPlaylist.label = match.captured("media");
         m_lineParamsPlaylist.numericParam = match.captured("numVal").toInt( & convOk);

         /* 'play' command requires 'media' to be specified */
         res = checkPlaylistParams( errorDetail, convOk);
      }
      else
      {
         m_lineKind = LINE_INVALID;
         errorDetail = QObject::tr( "<i>Command</i> is not: <b>play, pause, stop, rewind, fadeout, volume</b>");
      }
   }

   if (res == false)
   {
      m_lineKind = LINE_INVALID;
      m_errorString = QObject::tr("<b>syntax error.</b> "
                                  "<p>Usage: <b>playlist LINE <i>command [\"track label\"]</i></b></p>"
                                  "<p><i>command is: </i><b>play, pause, stop, rewind, fadeout, volume</b></p>"
                                  "<p>Track label (within quotes) is required for <i>play</i> and <i>show</i> commands.</p>");

      if (errorDetail != QString())
      {
         m_errorString += QString("<p>%1</p>").arg( errorDetail);
      }
   }

   return res;
}

bool LineParser::checkPlaylistParams( QString & errorDetail, bool validVolume)
{
   bool res = false;

   if ( (m_lineParamsPlaylist.subCommand == PLAYLIST_PLAY) &&
        (m_lineParamsPlaylist.label.isEmpty()) )
   {
      m_lineKind = LINE_INVALID;
      errorDetail = QObject::tr( "Command <b>play</b> requires to specify media");
   }
   else if ( (m_lineParamsPlaylist.subCommand == PLAYLIST_SHOW) &&
             (m_lineParamsPlaylist.label.isEmpty()) )
   {
      m_lineKind = LINE_INVALID;
      errorDetail = QObject::tr( "Command <b>show</b> requires to specify media");
   }
   else if (m_lineParamsPlaylist.subCommand == PLAYLIST_VOLUME)
   {
      if (validVolume == false)
      {
         // in this case, use -1 for default volume
         m_lineParamsPlaylist.numericParam = -1;
         res = true;
      }
      else if ((m_lineParamsPlaylist.numericParam < 0) || (m_lineParamsPlaylist.numericParam > 100))
      {
         m_lineKind = LINE_INVALID;
         errorDetail = QObject::tr( "Volume parameter, when present, must be in range 0-100.\n"
                                    "When missing, default volume is used");
      }
      else
      {
         res = true;
      }
   }
   else
   {
      /* all fine */
      res = true;
   }

   return res;
}

bool LineParser::parseLight( const QString & line)
{
   bool res = false;
   QRegularExpression * regexp = KeywordRegExpTable.value( "light");
   QRegularExpressionMatch match = regexp->match( line);
   QString errorDetail;

   if (match.hasMatch())
   {
      m_lineParamsLight.subCommand = LightSubcommandTable.value( match.captured("cmd"),
                                                                 LIGHT_NONE);

      if (m_lineParamsLight.subCommand == LIGHT_NONE)
      {
         m_lineKind = LINE_INVALID;
         errorDetail = QObject::tr("<i>Command</i> must be <b>trigger</b> or <b>stop</b>");
      }
      else
      {
         m_lineParamsLight.preset = match.captured("preset");

         if ((m_lineParamsLight.subCommand == LIGHT_TRIGGER) &&
             (m_lineParamsLight.preset.isEmpty()) )
         {
             errorDetail = QObject::tr("Command <b>trigger</b> requires a label");
         }
         else
         {
            res = true;
         }
      }
   }

   if (res == false)
   {
      m_lineKind = LINE_INVALID;
      m_errorString = QObject::tr("<b>syntax error.</b>. "
                                  "<p>Usage: <b>light <i>command  [preset]</i></b></p>"
                                  "<p><i>command</i> is <b>trigger</b> or <b>stop</b></p>");

      if (errorDetail != QString())
      {
         m_errorString += QString("<p>%1</p>").arg( errorDetail);
      }
   }

   return res;
}

bool LineParser::parseSequence( const QString & line)
{
   bool res = false;
   QRegularExpression * regexp = KeywordRegExpTable.value( "sequence");
   QRegularExpressionMatch match = regexp->match( line);

   m_errorString.clear();

   if (match.hasMatch())
   {
      m_lineParamsSequence.subCommand = SequenceSubcommandTable.value( match.captured("cmd"),
                                                                       SEQUENCE_NONE);

      if (m_lineParamsSequence.subCommand == SEQUENCE_NONE)
      {
         m_lineKind = LINE_INVALID;
         m_errorString += QObject::tr( "Command must be <i>pause</i> or <i>stop</i>");
      }
      else
      {
         bool timeValid = false;
         m_lineParamsSequence.timeSec = match.captured("time").toDouble(& timeValid);
         res = checkParamsForSequence( timeValid);
      }
   }
   else
   {
      m_lineKind = LINE_INVALID;
      m_errorString += QObject::tr( "<p>Usage: <b>sequence <i>command  [time]</i></b></p>"
                                    "<p><i>command</i> is <b>pause</b> or <b>stop</b></p>"
                                    "<p><i>time</i> is in seconds (minimum 0.001)</p>");
   }

   return res;
}

bool LineParser::parseHall( const QString & line)
{
   bool res = false;
   QRegularExpression * regexp = KeywordRegExpTable.value( "hall");
   QRegularExpressionMatch match = regexp->match( line);

   QString errorDetail;

   if (match.hasMatch())
   {
      m_lineParamsHall.subCommand = HallSubcommandTable.value( match.captured("cmd"),
                                                               HALL_NONE);

      if (m_lineParamsHall.subCommand != HALL_NONE)
      {
         /* store zone and level, if any */
         m_lineParamsHall.zone = match.captured("zone");
         m_lineParamsHall.level = match.captured("level").toInt();  /* set to 0 in case of error */

         /* if level not set, use full on */
         if (m_lineParamsHall.level == 0)
         {
            m_lineParamsHall.level = HALL_TRUN_ON_FULL;
         }

         res = checkParamsForHall();
      }
      else
      {
         m_lineKind = LINE_INVALID;
         errorDetail = QObject::tr( "<i>command</i> must be: <b>on</b> or <b>off</b>");
      }
   }
   else
   {
      m_lineKind = LINE_INVALID;
      errorDetail = QObject::tr("missing parameters ");
   }

   if (res == false)
   {
      m_errorString = QString("syntax error."
                              "<p>Usage: <b>hall command </i>\"zone\" level</i></b></p>"
                              "<p><b>command</b> is <b>on</b> or <b>off</b>; <b>level</b> is 0 to 10</p>");

      if (errorDetail != QString())
      {
         m_errorString += QString("<p>%1</p>").arg( errorDetail);
      }
   }

   return  res;
}


bool LineParser::parseEntry(const QString &line)
{
   bool res = false;
   QRegularExpression * regexp = KeywordRegExpTable.value( "entry");
   QRegularExpressionMatch match = regexp->match( line);

   m_errorString.clear();

   if (match.hasMatch())
   {
      m_lineParamsEntry.label = match.captured("label");

      if (m_lineParamsEntry.label == QString())
      {
         m_lineKind = LINE_INVALID;
         m_errorString += QObject::tr( "Command must be <i>entry \"label\":</i>");
      }
      else
      {
         res = true;
      }
   }
   else
   {
      m_lineKind = LINE_INVALID;
      m_errorString += QObject::tr( "<p>Usage: <i>entry \"label\":</i></p>");
   }

   return res;
}


bool LineParser::checkParamsForSequence(bool timeValid)
{
   bool paramsValid = false;


   if ((m_lineParamsSequence.subCommand == SEQUENCE_PAUSE) &&
       (timeValid == false))
   {
      m_lineKind = LINE_INVALID;
      m_errorString += QObject::tr( "Command <b>pause</b> requires a time (in seconds).</i>");
   }
   else if ((m_lineParamsSequence.subCommand == SEQUENCE_STOP) &&
            (timeValid == true))
   {
      /* 'stop' command does not require to specify a time */
      m_lineKind = LINE_INVALID;
      m_errorString += QObject::tr("<b>syntax error.</b>"
                                   "Command <b>stop</b> does not require a time");
   }
   else
   {
      paramsValid = true;
   }

   return paramsValid;
}

bool LineParser::checkParamsForHall()
{
   /* zone must always be present */
   if (m_lineParamsHall.zone == QString())
   {
      m_lineKind = LINE_INVALID;
      m_errorString = QObject::tr("<b>syntax error.</b> "
                                  "<i>zone</i> not found.");
   }

   return m_lineKind != LINE_INVALID;
}


}  // namespace Sequencer






