#include "OwnFileFormatter.h"
#include <QTextStream>
#include <QFile>

#include "testableAssert.h"
#include "OwnItem.h"

OwnFileFormatter::OwnFileFormatter(const QString & targetPath) :
   m_targetPath( targetPath)
{
}


const QString & OwnFileFormatter::format(const QList<OwnItem *> & ownItems)
{
   m_fileContents.clear(); // in case function is called more than once

   QTextStream stream( & m_fileContents);

   copyPreviousHeader( stream);
   formatVersion( stream);
   formatOpenWebNetItems( stream, ownItems);

   return m_fileContents;
}

void OwnFileFormatter::copyPreviousHeader( QTextStream & stream)
{
   /* try to open target file in read only to copy headers */
   QFile oldFile( m_targetPath);

   bool valid = oldFile.open( QIODevice::ReadOnly);

   if (valid)
   {
      bool goOn = true;
      QTextStream oldStream( & oldFile);

      while (goOn)
      {
         QString line = oldStream.readLine();
         QString trimmedLine = line.trimmed();

         // copy initial blank lines and commented lines
         goOn = (trimmedLine.startsWith('#') || trimmedLine.isEmpty());

         if (goOn)
         {
            stream << line << "\n";
         }

         /* also check end of file */
         goOn &= ( ! oldStream.atEnd());
      }

      oldFile.close();
   }
}

void OwnFileFormatter::formatVersion( QTextStream & stream)
{
   stream << "\n\n" << "OWN VERSION 1.0" << "\n\n";
}

void OwnFileFormatter::formatOpenWebNetItems( QTextStream & stream,
                                              const QList<OwnItem *> & ownItems)
{
   foreach (const OwnItem * item, ownItems)
   {
      T_ASSERT( item);

      stream << QString("\"%1\"  \t%2\t %3 \n")
                .arg(item->label)
                .arg( item->where)
                .arg( item->dimmerable ? "DIMM" : "ON_OFF");
   }
}

