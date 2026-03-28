
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/OpenWebNet/GUI/OwnActuatorWidget.h
   src/OpenWebNet/GUI/OwnActuatorWidget.cpp
   src/OpenWebNet/GUI/OperativeOwnDelegate.h
   src/OpenWebNet/GUI/OperativeOwnDelegate.cpp
   src/OpenWebNet/GUI/OpenWebNetPanel.ui
   src/OpenWebNet/GUI/OpenWebNetPanel.h
   src/OpenWebNet/GUI/OpenWebNetPanel.cpp
   src/OpenWebNet/GUI/OpenWebNetGUIFactory.h
   src/OpenWebNet/GUI/OpenWebNetGUIFactory.cpp
   src/OpenWebNet/GUI/EditModeOwnDelegate.h
   src/OpenWebNet/GUI/EditModeOwnDelegate.cpp
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)