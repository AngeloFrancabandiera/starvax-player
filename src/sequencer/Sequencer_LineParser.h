#ifndef SEQUENCER_LINE_PARSER_H
#define SEQUENCER_LINE_PARSER_H

#include <QString>

#include "Sequencer.h"

namespace Sequencer {

/** This is the parser for a single line of script.
 * It is made to be instanced once and used in a loop. Method 'parse' parses
 * a line and the result can be read immediately with dedicated methods.
 */
class LineParser
{
public:
   LineParser();
   ~LineParser();

   /**
    * parse given line; fill internal structures and return if the line is valid
    * @param line
    * @return TRUE for a legal line (even comment or blank); FALSE is a syntax error occourred
    */
   bool parseLine( const QString & line);

   /**
    * @return the error string relative to last parse operation. If \a parseLine returned true,
    *   this string is expected to be empty.
    */
   QString errorString() const {
      return m_errorString;
   }

   /** retrieve the kind of line of last parse operation. If parse operation failed,
    * \a LINE_BLANK is returned */
   enum LineKind getLineKind() const;

   const LineParamsRepeat & getLineParamsRepeat() const {
      return m_lineParamsRepeat;
   }

   const LineParamsPlaylist & getLineParamsPlaylist() const {
      return m_lineParamsPlaylist;
   }

   const LineParamsLight & getLineParamsLight() const {
      return m_lineParamsLight;
   }

   const LineParamsSequence & getLineParamsSequence() const {
      return m_lineParamsSequence;
   }

   const LineParamsHall & getLineParamsHall() const {
      return m_lineParamsHall;
   }

   const LineParamsEntry & getLineParamsEntry() const {
      return m_lineParamsEntry;
   }

private:
   LineKind m_lineKind;
   LineParamsRepeat m_lineParamsRepeat;
   LineParamsPlaylist m_lineParamsPlaylist;
   LineParamsLight m_lineParamsLight;
   LineParamsSequence m_lineParamsSequence;
   LineParamsHall m_lineParamsHall;
   LineParamsEntry m_lineParamsEntry;
   QString m_errorString;

private:
   /* parse functions */
   bool parseRepeat( const QString & line);
   bool parseEnd( const QString & line);
   bool parsePlaylist( const QString & line);
   bool parseLight( const QString & line);
   bool parseSequence( const QString & line);
   bool parseHall( const QString & line);
   bool parseEntry( const QString & line);

   bool checkParamsForSequence( bool timeValid);
   bool checkParamsForHall();
   bool checkPlaylistParams(QString & errorDetail, bool validVolume);
};

}  // namespace Sequencer

#endif  // SEQUENCER_LINE_PARSER_H

