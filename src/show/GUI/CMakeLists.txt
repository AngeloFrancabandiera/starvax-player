
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/show/GUI/ShowFileGui.cpp
   src/show/GUI/ShowFileGui.h
   src/show/GUI/StatusDisplay.h
   src/show/GUI/StatusDisplayQt.cpp
   src/show/GUI/StatusDisplayQt.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)
