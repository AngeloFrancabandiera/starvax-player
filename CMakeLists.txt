# Startvax MeTeOr player

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(starvax_player VERSION 2.5.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if (UNIX)
   set(CMAKE_PREFIX_PATH "/media/angelo/Windows7_OS/ANGELO/bin/Qt_6.9/6.9.1/gcc_64")
endif()

add_compile_definitions(
VERSION_MAJOR="2"
VERSION_MINOR="5"
VERSION_BUILD="0"
VERSION_STAGE="alpha3"
)

add_compile_definitions(
#   ssize_t=size_t
   STR_SPLIT=Qt
   )


IF(CMAKE_BUILD_TYPE MATCHES Debug)
  message("Debug build.")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
  message("Release build.")
  add_compile_definitions( QT_NO_DEBUG_OUTPUT)
ELSE()
  message("Unknown build type.")
ENDIF()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

set(QT_LIBRARIES
  Core
  Gui
  Widgets
  OpenGL
  Network
  Multimedia
  MultimediaWidgets
)


# Link Qt
find_package(QT NAMES Qt6 COMPONENTS
   Core
   Gui
   Widgets
   OpenGL
   Network
   Multimedia
   MultimediaWidgets
   REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
   Core
   Gui
   Widgets
   OpenGL
   Network
   Multimedia
   MultimediaWidgets
   REQUIRED)


find_package(usb REQUIRED)

qt_standard_project_setup(REQUIRES 6.9)


set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
  resources/HardCodedIcons.qrc
)


add_subdirectory(src)

include_directories(
   ${ALL_SRC_DIRS}
   resources
   )

if(WIN32)
add_executable(starvax_player  WIN32
    ${STARVAX_SOURCES}
)
endif()


if (UNIX)
add_executable(starvax_player
    ${STARVAX_SOURCES}
)
endif()


target_link_libraries(starvax_player PRIVATE
   Qt${QT_VERSION_MAJOR}::Core
   Qt${QT_VERSION_MAJOR}::Gui
   Qt${QT_VERSION_MAJOR}::Widgets
   Qt${QT_VERSION_MAJOR}::OpenGL
   Qt${QT_VERSION_MAJOR}::Network
   Qt${QT_VERSION_MAJOR}::Multimedia
   Qt${QT_VERSION_MAJOR}::MultimediaWidgets

   ${USB_LIBRARY}
   )


if(WIN32)
  add_custom_target( copy_deps
     COMMAND ${CMAKE_SOURCE_DIR}\\deploy\\win32_x64\\win32_post_build.bat ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
     )
endif()

if (UNIX)
  add_custom_target( copy_deps
     COMMAND cp -r ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}
     )
endif()
