
add_subdirectory(GUI)

set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/show/ApplicationSettings.cpp
   src/show/ApplicationSettings.h
   src/show/FileInport.cpp
   src/show/FileInport.h
   src/show/HtmlOptimizer.cpp
   src/show/HtmlOptimizer.h
   src/show/IF_ShowGuiInterface.h
   src/show/ShowFactory.cpp
   src/show/ShowFactory.h
   src/show/ShowFactory_IF.h
   src/show/ShowFileFactory.cpp
   src/show/ShowFileFactory.h
   src/show/ShowFileFormatter.cpp
   src/show/ShowFileFormatter.h
   src/show/ShowFileInfo.h
   src/show/showfileparser.cpp
   src/show/showfileparser.h
   src/show/ShowFileStrings.h
   src/show/ShowLoader.cpp
   src/show/ShowLoader.h
   src/show/ShowManager.cpp
   src/show/ShowManager.h
   src/show/ShowWriter.cpp
   src/show/ShowWriter.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)
