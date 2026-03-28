#include "FileInport.h"

#include <QFileInfo>
#include <QTextStream>
#include <qstandardpaths.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qapplication.h>

#include "supported_files.h"
#include "ApplicationSettings.h"



FileInport::FileInport( ApplicationSettings & applicationSettings,
                        QObject *parent) :
   QObject(parent),
   m_applicationSettings(applicationSettings),
   m_lastDropTarget( OTHER)
{
}

/*!
 * check file estention and emit a signal to triggerr
 * an appropriate handler
 */
bool FileInport::openFile( QString filename )
{
   QString ext = QFileInfo( filename ).suffix().toLower();
   bool result = true;

   if( SCRIPT_SUPPORTED_FORMATS.contains( ext ) )
   {
      /* script file */
      QString content = getFileContents(filename);
      emit inportScriptContent( content );
   }
   else if( MEDIA_TRACKS_SUPPORTED_FORMATS.contains( ext ) ||
            PICTURE_SUPPORTED_FORMATS.contains(ext))
   {
      /* media file */
      if (m_lastDropTarget == PLAYLIST_B)
      {
         emit inportMediaTracksLineB( QStringList(filename) );
      }
      else
      {
         /* playlist line A and default */
         emit inportMediaTracksLineA( QStringList(filename) );
      }

      m_lastDropTarget = OTHER;
   }
   else if( SHOWS_SUPPORTED_FORMATS.contains( ext ) )
   {
      /* meteor show file */
      emit inportShowFile( filename );
   }
   else if( ext == STYLE_SHEET_EXT )
   {
      /* style sheet */
      emit loadStyle( filename );
   }
   else
   {
      // unrecognised format or bad input url
      QMessageBox::warning( nullptr, tr(" Invalid operation "),
                            tr("Unable to open file of type: %1").arg(ext) );
      result = false;
   }

   return result;
}

QString FileInport::getFileContents(QString filename)
{
   QFile scriptFile( filename);
   QString content;

   bool res = scriptFile.open( QIODevice::ReadOnly);

   if (res == true)
   {
      QTextStream stream( &scriptFile);
      QString content = stream.readAll();
      scriptFile.close();
   }

   return content;
}

void FileInport::openScriptDialog()
{
   QString filename;
   QString doc_dir = m_applicationSettings.openScriptFolder();

   if (doc_dir == QString())
   {
      QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).at(0);
   }

   /* open from documents folder */
   filename = QFileDialog::getOpenFileName( nullptr, tr("Open script source"),
                                            doc_dir, SCRIPT_DLG_FILTER );

   if( filename != QString() )
   {
      openFile( filename);

      m_applicationSettings.setOpenScriptFolder( QFileInfo(filename).absoluteDir().absolutePath());
   }
}


void FileInport::openTracksDialogLineA()
{
   QStringList files = openTracksDialog( PLAYLIST_A);

   if( files.count() > 0)
   {
      emit inportMediaTracksLineA( files );
   }
}

void FileInport::openTracksDialogLineB()
{
   QStringList files = openTracksDialog( PLAYLIST_B);

   if( files.count() > 0)
   {
      emit inportMediaTracksLineB( files );
   }
}


QStringList FileInport::openTracksDialog( GuiArea line)
{
   QStringList filenames;
   int lineSetting = static_cast<int>(line);
   QString music_dir = m_applicationSettings.openMusicFolder( lineSetting);

   if (music_dir == QString())
   {
      QStandardPaths::standardLocations( QStandardPaths::MusicLocation ).at(0);
   }

   /* open from music folder */
   filenames = QFileDialog::getOpenFileNames( nullptr, tr("Open media - %1").
                                              arg((line==PLAYLIST_B) ? tr("LINE B") : tr("LINE A")),
                                              music_dir, MEDIA_TRACKS_DLG_FILTER );

   /* send audio files to playlist manager */
   if( filenames.count() > 0)
   {
      m_applicationSettings.setOpenMusicFolder( lineSetting,
                                                QFileInfo(filenames.last()).absoluteDir().absolutePath());
   }

   return filenames;
}

QString FileInport::open_style_dialog()
{
   QString filename;
   filename = QFileDialog::getOpenFileName( nullptr, tr("Open a style-sheet file"),
                                            qApp->applicationDirPath() + "/res/styles",
                                            STYLES_DLG_FILTER );
   return filename;
}
