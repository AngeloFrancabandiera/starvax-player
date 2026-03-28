
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/lightScene/DMX/DmxInterface_K8062.cpp
   src/lightScene/DMX/DmxInterface_K8062.h
   src/lightScene/DMX/dmxinterfacefactory_imp.cpp
   src/lightScene/DMX/DmxUsb.cpp
   src/lightScene/DMX/DmxUsb.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)