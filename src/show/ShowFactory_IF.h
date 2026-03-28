#ifndef SHOWFACTORY_IF_H
#define SHOWFACTORY_IF_H

#include <QObject>

class QAbstractListModel;

class ShowLoader;
class IF_ShowGuiInterface;
class IF_ShowWriterInterface;
class ShowFileFormatter;

class MediaListModel;
class LightPresetModel;
class SequenceModel;
class SequenceEditorGui;
class IF_ScriptEngineInterface;
class ApplicationSettings;


/**
 * @brief factory interface
 */
class IF_ShowFactoryInterface : public QObject
{
   Q_OBJECT
public:
   explicit IF_ShowFactoryInterface(QObject *parent = nullptr) : QObject(parent) {}
   virtual ~IF_ShowFactoryInterface() {}

   virtual IF_ShowGuiInterface *buildShowGui() = 0;

   virtual ShowLoader * buildShowLoader( IF_ShowGuiInterface *selectDialog,
                                         MediaListModel *mediaModelLineA,
                                         MediaListModel * mediaModelLineB,
                                         LightPresetModel *lightModel,
                                         SequenceEditorGui * sequencerGui,
                                         IF_ScriptEngineInterface *scriptInterafce,
                                         ApplicationSettings & appSettings) = 0;

   virtual IF_ShowWriterInterface *buildShowWriter(ShowFileFormatter *formatter) = 0;

   virtual ShowFileFormatter *buildShowFileFormatter(const IF_ScriptEngineInterface &script,
                                                     const QAbstractListModel &mediaModelLineA,
                                                     const QAbstractListModel &mediaModelLineB,
                                                     const QAbstractListModel &lightsetModel,
                                                     const SequenceEditorGui & sequencerGui,
                                                     ApplicationSettings & appSettings) = 0;

   virtual void deleteShowGui( IF_ShowGuiInterface *showGui) = 0;
   virtual void deleteShowLoader( ShowLoader * showLoader) = 0;
   virtual void deleteShowWriter( IF_ShowWriterInterface *showWriter) = 0;
   virtual void deleteShowFileFormatter( ShowFileFormatter *formatter) = 0;
};

#endif // SHOWFACTORY_IF_H

