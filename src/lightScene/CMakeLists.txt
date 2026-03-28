
add_subdirectory(DMX)
add_subdirectory(GUI)

set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/lightScene/DmxInterface.h
   src/lightScene/DmxInterfaceFactory.h
   src/lightScene/IF_DmxSliderPanel.h
   src/lightScene/IF_GuiLightControlPanel.h
   src/lightScene/IF_LightEngineInterface.h
   src/lightScene/IF_lighttransitionengine.h
   src/lightScene/LightEngine.cpp
   src/lightScene/LightEngine.h
   src/lightScene/LightFunctionFactory.cpp
   src/lightScene/LightFunctionFactory.h
   src/lightScene/LightPresetData.cpp
   src/lightScene/LightPresetData.h
   src/lightScene/LightPresetFactory.cpp
   src/lightScene/LightPresetFactory.h
   src/lightScene/LightPresetModel.cpp
   src/lightScene/LightPresetModel.h
   src/lightScene/lighttransitionengine.cpp
   src/lightScene/lighttransitionengine.h
  PARENT_SCOPE
)


set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)