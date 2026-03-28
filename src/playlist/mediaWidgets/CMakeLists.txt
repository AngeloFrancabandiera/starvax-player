
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/playlist/mediaWidgets/FullScreenMediaWidget.cpp
   src/playlist/mediaWidgets/FullScreenMediaWidget.h
   src/playlist/mediaWidgets/FullScreenMediaWidget_IF.h
   src/playlist/mediaWidgets/MediaListView.cpp
   src/playlist/mediaWidgets/MediaListView.h
   src/playlist/mediaWidgets/mediaPositionSlider.h
   src/playlist/mediaWidgets/MediaToolbar.h
   src/playlist/mediaWidgets/PlaylistGuiFactory.cpp
   src/playlist/mediaWidgets/PlaylistGuiFactory.h
   src/playlist/mediaWidgets/StillPictureWidget.cpp
   src/playlist/mediaWidgets/StillPictureWidget.h
   src/playlist/mediaWidgets/VideoWidget.cpp
   src/playlist/mediaWidgets/VideoWidget.h
   src/playlist/mediaWidgets/swiftslider.cpp
   src/playlist/mediaWidgets/swiftslider.h
   src/playlist/mediaWidgets/volumeslider.h
  PARENT_SCOPE
)


set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)
