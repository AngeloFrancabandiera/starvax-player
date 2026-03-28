add_subdirectory(GUI)

set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/script/HighLightTextViewer.cpp
   src/script/HighLightTextViewer.h
   src/script/IF_ActionSelectorInterface.h
   src/script/IF_ScriptEngineInterface.h
   src/script/IF_ScriptViewerInterface.h
   src/script/ScriptActionCreator.cpp
   src/script/ScriptActionCreator.h
   src/script/ScriptActionTrigger.h
   src/script/ScriptEngine.cpp
   src/script/ScriptEngine.h
   src/script/ScriptFunctionFactory.cpp
   src/script/ScriptFunctionFactory.h
   src/script/scriptViewer.cpp
   src/script/scriptViewer.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)
