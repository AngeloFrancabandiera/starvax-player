set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/lightScene/GUI/LevelViewForPreset.cpp
   src/lightScene/GUI/LevelViewForPreset.h
   src/lightScene/GUI/LightChannelWidget.cpp
   src/lightScene/GUI/LightChannelWidget.h
   src/lightScene/GUI/lightControlGui.ui
   src/lightScene/GUI/lightControlGuiFactory.cpp
   src/lightScene/GUI/lightControlGuiFactory.h
   src/lightScene/GUI/LightPresetDelegate.cpp
   src/lightScene/GUI/LightPresetDelegate.h
   src/lightScene/GUI/LightPresetPlaybar.cpp
   src/lightScene/GUI/LightPresetPlaybar.h
   src/lightScene/GUI/LightPresetPlaybar.ui
   src/lightScene/GUI/LightsetListView.cpp
   src/lightScene/GUI/LightsetListView.h
   src/lightScene/GUI/LightsetPainter.cpp
   src/lightScene/GUI/LightsetPainter.h
   src/lightScene/GUI/LightSlider.cpp
   src/lightScene/GUI/LightSlider.h
   src/lightScene/GUI/QtDmxSliderPanel.cpp
   src/lightScene/GUI/QtDmxSliderPanel.h
   src/lightScene/GUI/QtLightControlPanel.cpp
   src/lightScene/GUI/QtLightControlPanel.h
  PARENT_SCOPE
)


set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)