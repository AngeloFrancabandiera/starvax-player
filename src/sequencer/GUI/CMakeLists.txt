
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/sequencer/GUI/HtmlDelegate.cpp
   src/sequencer/GUI/HtmlDelegate.h
   src/sequencer/GUI/SequenceEditorGui.h
   src/sequencer/GUI/SequenceEditorGui.ui
   src/sequencer/GUI/SequenceEditorGuiQt.cpp
   src/sequencer/GUI/SequenceEditorGuiQt.h
   src/sequencer/GUI/Sequencer_CodeEditor.cpp
   src/sequencer/GUI/Sequencer_CodeEditor.h
   src/sequencer/GUI/SequencerErrorTableWidget.cpp
   src/sequencer/GUI/SequencerErrorTableWidget.h
   src/sequencer/GUI/SequencerHighlighter.cpp
   src/sequencer/GUI/SequencerHighlighter.h
  PARENT_SCOPE
)


set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)

