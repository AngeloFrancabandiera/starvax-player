#ifndef SHOWLOADER_H
#define SHOWLOADER_H

#include <array>
#include <QObject>

class IF_ShowGuiInterface;
class ShowFileParser;
class ShowFileFactory;
class QAbstractListModel;

class QTextStream;

class IF_ScriptEngineInterface;
class MediaListModel;
class LightPresetModel;
class SequenceEditorGui;
class ApplicationSettings;


class ShowLoader : public QObject
{
   Q_OBJECT
public:
   explicit ShowLoader( IF_ShowGuiInterface & fileSelect, ShowFileFactory *parserFactory ,
                        std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & mediaModelSet,
                        LightPresetModel * lighModel, SequenceEditorGui * sequencerGui,
                        IF_ScriptEngineInterface *scriptEngine,
                        ApplicationSettings & applicationSettings, QObject *parent = nullptr);
   ~ShowLoader();

public slots:
   void LoadShow( const QString & filePath);

signals:
   void scriptContent( const QString& script );
   void showNameChanged( const QString& filename );
   void newSearchPath( const QString& path );
   /** there were fixed errors in last show file */
   void loadShowCorrupted();

private:
   IF_ShowGuiInterface & m_guiInterface;
   ShowFileFactory * m_parserFactory;
   std::array<QAbstractListModel *, NUMBER_OF_MEDIA_DECKS> & m_mediaModelSet;
   LightPresetModel * m_lightModel;
   SequenceEditorGui * m_sequencerGui;
   IF_ScriptEngineInterface *m_scriptEngine;
   ApplicationSettings & m_applicationSettings;

private:
   bool confirmShowFileLoad(const ShowFileParser *parser) const;
   QString selectTargetFile(const QString &filePath) const;
   void loadShowFile(QTextStream *strShow, const QString &filename, ShowFileParser *parser);
   void loadExistingFile(const QString &fileName);
   void loadShowScript( QTextStream *strShow );
   void loadShowData(const QString &filename, const ShowFileParser &parser );
   void resetShowData();
};

#endif // SHOWLOADER_H
