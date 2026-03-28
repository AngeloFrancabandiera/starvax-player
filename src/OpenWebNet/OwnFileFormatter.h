#ifndef OWNFILEFORMATTER_H
#define OWNFILEFORMATTER_H

#include <QString>
class QTextStream;
class OwnItem;

/**
 * @brief The OwnFileFormatter class formats
 * an Open Web Net light file and produces a string.
 * If target file already exists, header is copied.
 */
class OwnFileFormatter
{
public:
   OwnFileFormatter( const QString & targetPath);

   /**
    * @brief format Open Web Net file. Nothing is written
    *   to target file (only header is copied)
    * @param ownItems holds items to be written
    * @return reference to target file contents.
    * NOTE: use returned reference before this class is deleted
    */
   const QString & format( const QList<OwnItem *> & ownItems);

private:
   QString m_targetPath;
   QString m_fileContents;

private:
   void copyPreviousHeader( QTextStream & stream);
   void formatVersion( QTextStream & stream);
   void formatOpenWebNetItems(QTextStream & stream, const QList<OwnItem *> & ownItems);
};

#endif // OWNFILEFORMATTER_H
