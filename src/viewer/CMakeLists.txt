
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/viewer/ActionListController.cpp
   src/viewer/ActionListController.h
   src/viewer/ActionListPainter.cpp
   src/viewer/ActionListPainter.h
   src/viewer/ActionListView.cpp
   src/viewer/ActionListView.h
   src/viewer/CMakeLists.txt
   src/viewer/modelViewRules.h
  PARENT_SCOPE
)


set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)