
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
  src/application/Action.cpp
  src/application/Action.h
  src/application/ActionModeFilter.cpp
  src/application/ActionModeFilter.h
  src/application/ApplicationFactory.cpp
  src/application/ApplicationFactory.h
  src/application/ExtAbstractListModel.h
  src/application/ITimerService.h
  src/application/main.cpp
  src/application/QtTimerService.cpp
  src/application/QtTimerService.h
  src/application/testableAssert.cpp
  src/application/testableAssert.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)

