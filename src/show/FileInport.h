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

   bool openFile( QString filename );
   static QString open_style_dialog();

   QStringList selectTracksForDeck( int deck);

public slots:
   void openScriptDialog();
   void openTracksDialogForDeck( int deck);


signals:
   void importScriptContent( QString filename );
   void importMediaTracksForDeck( QStringList filenames);
   void importShowFile( QString filename );
   void loadStyle( QString filename );

private:
   QString getFileContents(QString filename);

private:
   ApplicationSettings & m_applicationSettings;
};

#endif // FILEINPORT_H
