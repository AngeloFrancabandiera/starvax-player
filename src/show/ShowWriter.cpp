#include <QFileInfo>
#include <QTextStream>

#include "ShowWriter.h"
#include "ShowFileFormatter.h"
#include "IF_ShowGuiInterface.h"
#include "testableAssert.h"

ShowWriter::ShowWriter(ShowFileFormatter *formatter, QObject *parent) :
   IF_ShowWriterInterface(parent),
   m_formatter(formatter)
{
   T_ASSERT( formatter != nullptr);
}

/**
 * write the show to the given \p filePath. If such path is empty,
 * prompt the user to select a file.
 */
void ShowWriter::writeShow( IF_ShowGuiInterface &guiInterafce, const QString &filePath)
{
   QString targetFile;
   QFileInfo filePathInfo(filePath);

   if (filePathInfo.exists() && filePathInfo.isFile())
   {
      targetFile = filePath;
   }
   else
   {
      targetFile = guiInterafce.selectShowToSave();
   }

   if (targetFile != QString())
   {
      m_formatter->setFolder( QFileInfo(targetFile).absoluteDir());

      QString fileContent;
      fileContent = m_formatter->formatShowFile();

      writeToFile( targetFile, fileContent);
   }
}

/**
 * @param fileName - the full path of file, supposed to be correct
 * @param content - full file content
 */
void ShowWriter::writeToFile(const QString &fileName, const QString &content)
{
   QFile showFile( fileName);
   bool res = showFile.open( QIODevice::WriteOnly );

   T_ASSERT_REP ((res == true) && (showFile.isOpen()),
                 QString( tr("Unable to save to file: %1").arg(fileName)));

   QTextStream strShow( &showFile );

   strShow << content;
   showFile.close();

   m_savedFilePath = fileName;
}

