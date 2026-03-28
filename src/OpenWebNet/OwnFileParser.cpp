#include "OwnFileParser.h"
#include "testableAssert.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegularExpression>

namespace  {

const QString SUPPORTED_VERSION_LINE("OWN VERSION 1.0");

QRegularExpression LIGHT_POINT_REG_EX("\"(.*)\"\\s+(#?\\d+)\\s+(DIMM|ON_OFF)");
}  // namespace


OwnFileParser::OwnFileParser()
{
}

OwnFileParser::~OwnFileParser()
{
   m_items.clear();
   m_errors.clear();
}

void OwnFileParser::parseFile(const QString & fullPath)
{
   /* allow function to be called more than once*/
   m_items.clear();
   m_errors.clear();

   if (QFileInfo(fullPath).isFile())
   {
      QFile ownFile( fullPath);
      bool ok = ownFile.open( QIODevice::ReadOnly);
      T_ASSERT( ok);

      QTextStream stream( & ownFile);
      parseValidFile( stream);
      ownFile.close();
   }
   else
   {
      m_errors << QString("File %1 can not be opened.").arg(fullPath);
   }
}

void OwnFileParser::parseValidFile( QTextStream & stream)
{
   // check file version on first non commented line
   QString version = readValidLine( stream);

   if (version == SUPPORTED_VERSION_LINE)
   {
      parseLightPoints( stream);
   }
   else
   {
      m_errors << QObject::tr("Wrong file version: %1. Expected: %2")
                  .arg( version).arg( SUPPORTED_VERSION_LINE);
   }
}

QString OwnFileParser::readValidLine( QTextStream & stream)
{
   QString line;
   bool lineIsValid = false;

   /* skip comments and empty lines */
   do
   {
      line = stream.readLine();
      line = line.trimmed();

      lineIsValid = (line.size() > 0) && ( ! line.startsWith('#'));
   } while( (! stream.atEnd()) && ( ! lineIsValid));

   return line;
}

void OwnFileParser::parseLightPoints( QTextStream & stream)
{
   QString line;

   while ( ! stream.atEnd())
   {
      line = readValidLine( stream).trimmed();

      if (line.size() > 0)
      {
         parseLine( line);
      }
   }
}

void OwnFileParser::parseLine( const QString & line)
{
   QRegularExpressionMatch match = LIGHT_POINT_REG_EX.match( line);

   if (match.hasMatch())
   {
      QString label = match.captured(1);
      QString where = match.captured(2);
      bool dimmerable = (match.captured(3) == "DIMM");

      m_items << new OwnItem( label, where, dimmerable);
   }
   else
   {
      m_errors << QObject::tr("Invalid line: %1").arg( line);
   }
}




