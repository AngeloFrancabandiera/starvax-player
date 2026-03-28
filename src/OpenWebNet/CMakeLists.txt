
add_subdirectory(GUI)

set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/OpenWebNet/OpenWebNetEngine.cpp
   src/OpenWebNet/OpenWebNetEngine.h
   src/OpenWebNet/OpenWebNetEngine_IF.h
   src/OpenWebNet/OpenWebNetFactory.cpp
   src/OpenWebNet/OpenWebNetFactory.h
   src/OpenWebNet/OwnFileFormatter.cpp
   src/OpenWebNet/OwnFileFormatter.h
   src/OpenWebNet/OwnFileLoader.cpp
   src/OpenWebNet/OwnFileLoader.h
   src/OpenWebNet/OwnFileParser.cpp
   src/OpenWebNet/OwnFileParser.h
   src/OpenWebNet/OwnItem.cpp
   src/OpenWebNet/OwnItem.h
   src/OpenWebNet/OwnModel.cpp
   src/OpenWebNet/OwnModel.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)
