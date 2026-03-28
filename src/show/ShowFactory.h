#ifndef SHOWFACTORY_H
#define SHOWFACTORY_H

#include <ShowFactory_IF.h>

class QTextStream;
class QDir;
class QTextDocument;
class ShowFileParser;



/**
 * @brief Concrete factory that builds objects when the user
 *    issues commands to save or load a show.
 * Note that target files have not been selected yet.
 */
class ShowFactory : public IF_ShowFactoryInterface
{
   Q_OBJECT
public:
   explicit ShowFactory(int numOfLightChannels, ApplicationSettings & appSettings,
                        QObject *parent = nullptr);

   IF_ShowGuiInterface *buildShowGui() override;

   ShowLoader * buildShowLoader( IF_ShowGuiInterface *selectDialog,
                                 MediaListModel *mediaModelLineA, MediaListModel * mediaModelLineB,
                                 LightPresetModel *lightModel,
                                 SequenceEditorGui * sequencerGui,
                                 IF_ScriptEngineInterface *scriptInterafce,
                                 ApplicationSettings & appSettings) override;

   IF_ShowWriterInterface *buildShowWriter(ShowFileFormatter *formatter) override;

   ShowFileFormatter *buildShowFileFormatter(const IF_ScriptEngineInterface &script,
                                             const QAbstractListModel &mediaModelLineA,
                                             const QAbstractListModel &mediaModelLineB,
                                             const QAbstractListModel &lightsetModel,
                                             const SequenceEditorGui & sequencerGui,
                                             ApplicationSettings &appSettings) override;

   void deleteShowGui(IF_ShowGuiInterface *showGui) override;
   void deleteShowLoader(ShowLoader *showLoader) override;
   void deleteShowWriter(IF_ShowWriterInterface *showWriter) override;
   void deleteShowFileFormatter(ShowFileFormatter *formatter) override;

private:
   int m_numOfLightChannels;
   ApplicationSettings & m_appSettings;
};

#endif // SHOWFACTORY_H
