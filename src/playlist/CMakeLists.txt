
add_subdirectory(mediaWidgets)

set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/playlist/AbstractMediaSource.h
   src/playlist/AudioVideoSource.cpp
   src/playlist/AudioVideoSource.h
   src/playlist/ExponentialFader.cpp
   src/playlist/ExponentialFader.h
   src/playlist/Fader.cpp
   src/playlist/Fader.h
   src/playlist/IF_MediaEngineInterface.h
   src/playlist/MediaAutomation.cpp
   src/playlist/MediaAutomation.h
   src/playlist/MediaEngineImp.cpp
   src/playlist/MediaEngineImp.h
   src/playlist/MediaListModel.cpp
   src/playlist/MediaListModel.h
   src/playlist/MediaSourceFactory.cpp
   src/playlist/MediaSourceFactory.h
   src/playlist/PictureSource.cpp
   src/playlist/PictureSource.h
   src/playlist/PlaylistConstants.h
   src/playlist/PlaylistFunctionFactory.cpp
   src/playlist/PlaylistFunctionFactory.h
   src/playlist/playlistPainter.cpp
   src/playlist/playlistPainter.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)

