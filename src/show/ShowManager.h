#ifndef SHOWMANAGER_H
#define SHOWMANAGER_H

#include <QObject>

#include "LightPresetData.h"


class ShowFileParser;
class QTextStream;
class SequenceEditorGui;
class MediaListModel;
class LightPresetModel;
class ApplicationSettings;
class IF_ShowFactoryInterface;
class IF_ScriptEngineInterface;

/**
 * This class is supposed to be instantiated once for the whole
 * application lifetime and it's responsible to create, on demand,
 * the objects needed to open or save a Show file.
 */
class ShowManager : public QObject
{
   Q_OBJECT
public:
   explicit ShowManager( IF_ShowFactoryInterface *showFactory, IF_ScriptEngineInterface & scriptEngine,
                         MediaListModel & mediaModelLineA, MediaListModel & mediaModelLineB,
                         LightPresetModel &lightModel,SequenceEditorGui & sequencerGui,
                         ApplicationSettings & applicationSettings,
                         QObject *parent = nullptr);

public slots:
   void onSaveShowRequest();
   void onSaveShowAsRequest();
   void openFile();
   void loadShowFile( const QString &filename );

private:
   void saveShow( const QString &filename );

private:
   QString m_showFilename;
   MediaListModel & m_mediaModelLineA;
   MediaListModel & m_mediaModelLineB;
   LightPresetModel & m_lightsetModel;
   SequenceEditorGui & m_sequencerGui;

   IF_ScriptEngineInterface & m_scriptEngine;
   IF_ShowFactoryInterface *m_showFactory;

   ApplicationSettings & m_applicationSettings;

signals:
   void scriptContent( const QString& script);
   void showSaved( const QString & savedPath);
   void showNameChanged( const QString& filename);
   void newSearchPath( const QString& path);
   void loadShowCorrupted();

private slots:
   void onShowNameChanged(  const QString& filename);
};

#endif // SHOWMANAGER_H
