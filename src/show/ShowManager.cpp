#include "ShowManager.h"

#include "qfileinfo.h"
#include "qfile.h"
#include "qtextstream.h"

#include "ApplicationSettings.h"
#include "qstandardpaths.h"

#include "ShowFileGui.h"
#include "supported_files.h"
#include "showfileparser.h"
#include "ShowLoader.h"
#include "ShowFileFormatter.h"
#include "ShowFactory.h"
#include "ShowWriter.h"

#include "LightPresetModel.h"
#include "MediaListModel.h"
#include "ScriptEngine.h"


ShowManager::ShowManager( IF_ShowFactoryInterface *showFactory, IF_ScriptEngineInterface &scriptEngine,
                          MediaListModel &mediaModelLineA, MediaListModel & mediaModelLineB,
                          LightPresetModel &lightModel, SequenceEditorGui &sequencerGui,
                          ApplicationSettings & applicationSettings,  QObject *parent) :
   QObject(parent),
   m_mediaModelLineA( mediaModelLineA),
   m_mediaModelLineB( mediaModelLineB),
   m_lightsetModel(lightModel),
   m_sequencerGui( sequencerGui),
   m_scriptEngine(scriptEngine),
   m_showFactory(showFactory),
   m_applicationSettings(applicationSettings)
{
}

void ShowManager::onSaveShowRequest()
{
   saveShow( m_showFilename);
}

void ShowManager::onSaveShowAsRequest()
{
   saveShow( QString());
}

/**
 * save all show info, (script, light presets, media track, sequence actions, ...)
 * to \p filename.
 * @param filename - full path where to store show.
 */
void ShowManager::saveShow(const QString &filename)
{
   IF_ShowGuiInterface *guiInterafce = m_showFactory->buildShowGui();
   ShowFileFormatter *formatter = m_showFactory->buildShowFileFormatter( m_scriptEngine, m_mediaModelLineA,
                                                                         m_mediaModelLineB, m_lightsetModel,
                                                                         m_sequencerGui,
                                                                         m_applicationSettings);
   IF_ShowWriterInterface *writer = m_showFactory->buildShowWriter( formatter);

   try
   {
      writer->writeShow( *guiInterafce, filename);
      m_showFilename = writer->savedFilePath();

      if (m_showFilename != QString())
      {
         m_applicationSettings.addRecentShowFile( m_showFilename);
         emit showSaved( m_showFilename);
      }
   }
   catch(QString & error)
   {
      guiInterafce->notifyErrorMessage( tr("Error during file saving:"), error);
   }

   m_showFactory->deleteShowWriter( writer);
   m_showFactory->deleteShowFileFormatter( formatter);
   m_showFactory->deleteShowGui( guiInterafce);
}


void ShowManager::openFile()
{
   loadShowFile( QString());
}

/**
 * parse given \a filename to load script and tracks
 * \p filename full path of show file
 *
 * @param filename - may be empty. In this case, user is prompted to select a file
 */
void ShowManager::loadShowFile( const QString &filename )
{
   IF_ShowGuiInterface *guiInterafce = m_showFactory->buildShowGui();
   ShowLoader *loader = m_showFactory->buildShowLoader( guiInterafce, &m_mediaModelLineA,
                                                        &m_mediaModelLineB, &m_lightsetModel,
                                                        &m_sequencerGui,
                                                        &m_scriptEngine,
                                                        m_applicationSettings);

   connect( loader, SIGNAL(scriptContent(QString)), this, SIGNAL(scriptContent(QString)) );
   connect( loader, SIGNAL(showNameChanged(QString)), this, SLOT(onShowNameChanged(QString)) );
   connect( loader, SIGNAL(newSearchPath(QString)), this, SIGNAL(newSearchPath(QString)) );
   connect( loader, SIGNAL(loadShowCorrupted()), this, SIGNAL(loadShowCorrupted()));

   try
   {
      guiInterafce->showLoadingMessage();

      loader->LoadShow( filename);
      guiInterafce->hideLoadingMessage();
   }
   catch(QString & error)
   {
      guiInterafce->notifyErrorMessage( tr("Error during file loading:"), error);
   }

   m_showFactory->deleteShowLoader( loader);
   m_showFactory->deleteShowGui( guiInterafce);
}

void ShowManager::onShowNameChanged(const QString &filename)
{
   m_showFilename = filename;
   emit showNameChanged( filename);
}


