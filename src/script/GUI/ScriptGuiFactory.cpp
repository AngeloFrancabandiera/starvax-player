#include "ScriptGuiFactory.h"

#include "scriptViewer.h"
#include "HighLightTextViewer.h"
#include "ScriptActionLauncher.h"
#include "ScriptActionTrigger.h"
#include "ScriptActionCreator.h"
#include "ActionHoverDetection.h"
#include "ActionTargetSelectorDialog.h"
#include "SearchTextBar.h"
#include "ScriptEditToolbar.h"
#include "testableAssert.h"

#include <QLayout>


ScriptGuiFactory::ScriptGuiFactory(QWidget *parent) :
   QObject(parent)
{
}

IF_ScriptViewerInterface *ScriptGuiFactory::build( QWidget *container, ScriptActionTrigger *actionTrigger,
                                                   QAbstractListModel *mediaModelLineA,
                                                   QAbstractListModel *mediaModelLineB,
                                                   QStringListModel & sequenceEntryModel,
                                                   StatusDisplay * msgDisplay,
                                                   ApplicationSettings & settings,
                                                   QAbstractListModel *lightModel)
{
   T_ASSERT(container->layout() == nullptr);
   container->setLayout( new QVBoxLayout());
   container->layout()->setContentsMargins( 0,0,0,0);

   HighLightTextViewer *viewer = new HighLightTextViewer( *msgDisplay, settings, container);
   ActionTargetSelectorDialog *selectDialog = new ActionTargetSelectorDialog( mediaModelLineA, mediaModelLineB,
                                                                              lightModel, sequenceEntryModel,
                                                                              viewer);
   ScriptActionCreator *actionCreator = new ScriptActionCreator( *selectDialog);
   SearchTextBar * searchBar = new SearchTextBar( container);
   ScriptActionLauncher *actionLaunchDialog = new ScriptActionLauncher( *actionTrigger, viewer);
   ActionHoverDetection * actionDetector = new ActionHoverDetection( actionLaunchDialog, viewer);

   ScriptEditToolbar * editToolbar = new ScriptEditToolbar( settings, container);
   ScriptViewer * scriptViewer = new ScriptViewer( *viewer, *actionCreator, *searchBar,
                                                   *editToolbar, this);

   searchBar->setVisible( false);
   editToolbar->setVisible( false);

   container->layout()->addWidget( editToolbar);
   container->layout()->addWidget( viewer);
   container->layout()->addWidget( searchBar);

   connect( viewer, & HighLightTextViewer::filteredMouseMoveEvent,
            actionDetector, & ActionHoverDetection::onMouseMoveFiltered);
   connect( viewer, & HighLightTextViewer::loadShowRequest,
            scriptViewer, & ScriptViewer::loadShowRequest);

   connect( searchBar, & SearchTextBar::searchTextForward,
            viewer, & HighLightTextViewer::onSearchForwardReq);
   connect( searchBar, & SearchTextBar::searchTextBackward,
            viewer, & HighLightTextViewer::onSearchBackwardReq);
   connect( viewer, & HighLightTextViewer::textNotFound,
            searchBar, & SearchTextBar::onTextNotFound);

   connect( viewer, & HighLightTextViewer::currentCharFormatChanged,
            editToolbar, & ScriptEditToolbar::onEditorCurrentCharFormatChanged);

   connect( viewer, & HighLightTextViewer::undoAvailable,
            editToolbar, & ScriptEditToolbar::onUndoAvailable);

   connect( viewer, & HighLightTextViewer::redoAvailable,
            editToolbar, & ScriptEditToolbar::onRedoAvailable);

   connect( viewer, & HighLightTextViewer::alignmentChanged,
            editToolbar, & ScriptEditToolbar::onAlignmentChanged);

   connect( editToolbar, & ScriptEditToolbar::fontColorChanged,
            viewer, & HighLightTextViewer::setCursorColor);

   connect( editToolbar, & ScriptEditToolbar::boldChanged,
            viewer, & HighLightTextViewer::setCursorBold);

   connect( editToolbar, & ScriptEditToolbar::italicChanged,
            viewer, & HighLightTextViewer::setCursorItalic);

   connect( editToolbar, & ScriptEditToolbar::underlineChanged,
            viewer, & HighLightTextViewer::setCursorUnderline);

   connect( editToolbar, & ScriptEditToolbar::fontFaceChanged,
            viewer, & HighLightTextViewer::setCursorFontFamily);

   connect( editToolbar, & ScriptEditToolbar::fontSizeChanged,
            viewer, & HighLightTextViewer::setCursorFontSize);

   connect( editToolbar, & ScriptEditToolbar::alignRequest,
            viewer, & HighLightTextViewer::onAlignRequest);

   connect( editToolbar, & ScriptEditToolbar::undo,
            viewer, & HighLightTextViewer::undo);

   connect( editToolbar, & ScriptEditToolbar::redo,
            viewer, & HighLightTextViewer::redo);

   connect( editToolbar, & ScriptEditToolbar::copyFormatRequest,
            viewer, & HighLightTextViewer::onCopyFormatRequest);

   connect( editToolbar, & ScriptEditToolbar::pasteFormatRequest,
            viewer, & HighLightTextViewer::onPasteFormatRequest);

   return scriptViewer;
}
