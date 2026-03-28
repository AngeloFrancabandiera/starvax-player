
add_subdirectory(application)
add_subdirectory(dialogs)
add_subdirectory(lightScene)
add_subdirectory(OpenWebNet)
add_subdirectory(playlist)
add_subdirectory(script)
add_subdirectory(sequencer)
add_subdirectory(server)
add_subdirectory(show)
add_subdirectory(viewer)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   PARENT_SCOPE
)


set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
  PARENT_SCOPE
)
