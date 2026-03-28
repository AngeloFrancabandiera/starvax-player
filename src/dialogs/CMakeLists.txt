
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/dialogs/ActionTargetSelectorDialog.cpp
   src/dialogs/ActionTargetSelectorDialog.h
   src/dialogs/ActionTargetSelectorDialog.ui
   src/dialogs/AppSettingsGui.cpp
   src/dialogs/AppSettingsGui.h
   src/dialogs/AppSettingsGui.ui
   src/dialogs/AudioVideoPlayBar.cpp
   src/dialogs/AudioVideoPlayBar.h
   src/dialogs/AudioVideoPlayBar.ui
   src/dialogs/LightSetEditor.cpp
   src/dialogs/LightSetEditor.h
   src/dialogs/LightSetEditor.ui
   src/dialogs/mainwindow.cpp
   src/dialogs/mainwindow.h
   src/dialogs/mainwindow.ui
   src/dialogs/PicturePlaybar.cpp
   src/dialogs/PicturePlaybar.h
   src/dialogs/PicturePlaybar.ui
   src/dialogs/PlaylistBar.cpp
   src/dialogs/PlaylistBar.h
   src/dialogs/ScriptActionLauncher.cpp
   src/dialogs/ScriptActionLauncher.h
   src/dialogs/ScriptActionLauncher.ui
   src/dialogs/ScriptEditToolbar.cpp
   src/dialogs/ScriptEditToolbar.h
   src/dialogs/ScriptEditToolbar.ui
   src/dialogs/SearchTextBar.cpp
   src/dialogs/SearchTextBar.h
   src/dialogs/SearchTextBar.ui
   src/dialogs/ShowFileReportDialog.cpp
   src/dialogs/ShowFileReportDialog.h
   src/dialogs/ShowFileReportDialog.ui
  PARENT_SCOPE
)


set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)