
add_subdirectory(commands)
add_subdirectory(GUI)

set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/server/Command_IF.h
   src/server/CommandReply_IF.h
   src/server/MessageEngine.h
   src/server/MessageParser.cpp
   src/server/MessageParser.h
   src/server/NetworkCommandReply.cpp
   src/server/NetworkCommandReply.h
   src/server/NetworkInterface.h
   src/server/NetworkInterfaceQt.cpp
   src/server/NetworkInterfaceQt.h
   src/server/ServerEngine.cpp
   src/server/ServerEngine.h
   src/server/ServerFactory.cpp
   src/server/ServerFactory.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)
