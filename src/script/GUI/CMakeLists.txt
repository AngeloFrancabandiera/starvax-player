
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/script/GUI/ActionHoverDetection.h
   src/script/GUI/ScriptGuiFactory.cpp
   src/script/GUI/ActionHoverDetection.cpp
   src/script/GUI/ScriptGuiFactory.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)