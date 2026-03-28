#ifndef FILEINPORT_H
#define FILEINPORT_H

#include "qstring.h"
#include "qstringlist.h"
#include <QObject>

class ApplicationSettings;


class FileInport : public QObject
{
   Q_OBJECT
public:
   explicit FileInport( ApplicationSettings & applicationSettings,
                        QObject *parent = 0);

   /** Panel where drop operation occurred */
   enum GuiArea
   {
      PLAYLIST_A = 0,
      PLAYLIST_B,
      SCRIPT,
      SEQUENCE,
      LIGHT,
      OTHER
   };

   bool openFile( QString filename );
   static QString open_style_dialog();

   /** set panel in which last drop operation occurred */
   void setDropTarget( GuiArea target) {
      m_lastDropTarget = target;
   }

public slots:
   void openScriptDialog();
   void openTracksDialogLineA();
   void openTracksDialogLineB();


signals:
   void inportScriptContent( QString filename );
   void inportMediaTracksLineA( QStringList filenames );
   void inportMediaTracksLineB( QStringList filenames );
   void inportShowFile( QString filename );
   void loadStyle( QString filename );

private:
   QString getFileContents(QString filename);
   QStringList openTracksDialog( GuiArea line);

private:
   ApplicationSettings & m_applicationSettings;
   GuiArea m_lastDropTarget;
};

#endif // FILEINPORT_H
