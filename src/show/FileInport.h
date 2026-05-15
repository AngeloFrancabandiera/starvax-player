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
      PLAYLIST = 0,
      SCRIPT,
      SEQUENCE,
      LIGHT,
      OTHER
   };

   bool openFile( QString filename );
   static QString open_style_dialog();

   /** set panel in which last drop operation occurred */
   // _TODO ma serve davvero?
   void setDropTarget( GuiArea target, int param) {
      m_lastDropTarget = target;
      m_lastDropTargetParam = param;
   }

public slots:
   void openScriptDialog();
   void openTracksDialogForDeck( int deck);


signals:
   void importScriptContent( QString filename );
   void importMediaTracksForDeck( QStringList filenames, int deck);
   void importShowFile( QString filename );
   void loadStyle( QString filename );

private:
   QString getFileContents(QString filename);
   QStringList selectTracksForDeck( int deck);

private:
   ApplicationSettings & m_applicationSettings;
   GuiArea m_lastDropTarget;
   int m_lastDropTargetParam;
};

#endif // FILEINPORT_H
