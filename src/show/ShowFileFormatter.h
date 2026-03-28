#ifndef SHOWFILEFORMATTER_H
#define SHOWFILEFORMATTER_H

#include "qstring.h"
#include "QDir"

class QTextStream;
class QAbstractListModel;
class SequenceEditorGui;
class IF_ScriptEngineInterface;
class HtmlOptimizer;

/**
 * This class is in charge to translate data in models
 * into a string to be stored in a text file.
 */
class ShowFileFormatter
{
public:
   ShowFileFormatter( const IF_ScriptEngineInterface &script,
                      const QAbstractListModel & mediaModelLineA,
                      const QAbstractListModel & mediaModelLineB,
                      const QAbstractListModel & lightsetModel,
                      HtmlOptimizer & optimizer,
                      const SequenceEditorGui & sequencerGui);

   /* folder is not a collaborator, but is an option */
   void setFolder( const QDir & showFolder);

   QString formatShowFile();

private:
   QDir m_showPath;
   const IF_ScriptEngineInterface & m_script;
   const QAbstractListModel & m_mediaModelLineA;
   const QAbstractListModel & m_mediaModelLineB;
   const QAbstractListModel & m_lightsetModel;
   HtmlOptimizer & m_optimizer;
   const SequenceEditorGui & m_sequencerGui;

private:
   void formatPlaylist(QTextStream &output);
   void formatPlaylistForLine( QTextStream & output,
                               const QAbstractListModel & m_mediaModel,
                               const QString & openTag, const QString & closeTag);
   void formatLightTransitions(QTextStream &output);
   void formatSequencer(QTextStream &output);
   void formatScript(QTextStream &output);
};

#endif // SHOWFILEFORMATTER_H
