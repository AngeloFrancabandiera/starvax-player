#ifndef OWNFILEPARSER_H
#define OWNFILEPARSER_H

#include <QStringList>
#include "OwnItem.h"

class QTextStream;

/** this class parses an Open Web Net light file
 * and extracts light information.
 */
class OwnFileParser
{
public:
   OwnFileParser();
   ~OwnFileParser();

   /**
    * @brief parse a file to extract information.
    * Must be called before other functions of this file
    * @param fullPath
    */
   void parseFile( const QString & fullPath);

   /**
    * @brief retrieves errors from previous call to \a parseFile.
    * @return a list of errors. If last operation was smooth, the list
    *   is empty.
    */
   const QStringList & errors() const {
      return m_errors;
   }

   /**
    * @return a list of Open Web Net lights extracted from previous
    * parse operation made by \a parseFile
    */
   const QList<const OwnItem *> & openWebNetItems() const {
      return m_items;
   }

private:
   QList<const OwnItem *> m_items;
   QStringList m_errors;

private:
   void parseValidFile( QTextStream & stream);
   QString readValidLine( QTextStream & stream);
   void parseLightPoints( QTextStream & stream);
   void parseLine( const QString & line);
};

#endif // OWNFILEPARSER_H
