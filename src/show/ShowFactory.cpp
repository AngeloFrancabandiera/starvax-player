#include "ShowFactory.h"

#include "ShowFileGui.h"
#include "ShowLoader.h"
#include "showfileparser.h"
#include "ShowWriter.h"
#include "ShowFileFormatter.h"
#include "ShowFileFactory.h"
#include "testableAssert.h"
#include "HtmlOptimizer.h"
#include "ApplicationSettings.h"


ShowFactory::ShowFactory( int numOfLightChannels,
                          ApplicationSettings & appSettings, QObject *parent) :
   IF_ShowFactoryInterface(parent),
   m_numOfLightChannels(numOfLightChannels),
   m_appSettings( appSettings)
{
}

IF_ShowGuiInterface *ShowFactory::buildShowGui()
{
   return new ShowFileGui( m_appSettings);
}

ShowLoader *ShowFactory::buildShowLoader( IF_ShowGuiInterface *selectDialog, MediaListModel *mediaModelLineA,
                                          MediaListModel *mediaModelLineB, LightPresetModel *lightModel,
                                          SequenceEditorGui *sequencerGui,
                                          IF_ScriptEngineInterface *scriptInterafce,
                                          ApplicationSettings & appSettings)
{
   ShowFileFactory *parserFactory = new ShowFileFactory( m_numOfLightChannels, this);
   ShowLoader *loader = new ShowLoader( *selectDialog, parserFactory, mediaModelLineA,
                                        mediaModelLineB, lightModel, sequencerGui,
                                        scriptInterafce, appSettings, this);

   return loader;
}


IF_ShowWriterInterface *ShowFactory::buildShowWriter( ShowFileFormatter *formatter)
{
   return new ShowWriter( formatter, this);
}


ShowFileFormatter *ShowFactory::buildShowFileFormatter( const IF_ScriptEngineInterface &script,
                                                        const QAbstractListModel & mediaModelLineA,
                                                        const QAbstractListModel & mediaModelLineB,
                                                        const QAbstractListModel & lightsetModel,
                                                        const SequenceEditorGui &sequencerGui,
                                                        ApplicationSettings & appSettings)
{
   HtmlOptimizer * optimizer = new HtmlOptimizer(this);
   optimizer->setSaveScriptFontSize( appSettings.getSaveFontSize());

   return new ShowFileFormatter( script, mediaModelLineA, mediaModelLineB,
                                 lightsetModel, *optimizer, sequencerGui);
}


void ShowFactory::deleteShowGui(IF_ShowGuiInterface *showGui)
{
   T_ASSERT( showGui != nullptr);
   delete showGui;
}

void ShowFactory::deleteShowLoader(ShowLoader *showLoader)
{
   T_ASSERT( showLoader != nullptr);
   delete showLoader;
}

void ShowFactory::deleteShowWriter(IF_ShowWriterInterface *showWriter)
{
   T_ASSERT( showWriter != nullptr);
   delete showWriter;
}

void ShowFactory::deleteShowFileFormatter(ShowFileFormatter *formatter)
{
   T_ASSERT( formatter != nullptr);
   delete formatter;
}
