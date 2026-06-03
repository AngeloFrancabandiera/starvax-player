# Startvax MeTeOr player

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(starvax_player VERSION 2.5.2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set( REL_VERSION_MAJOR 2)
set( REL_VERSION_MINOR 5)
set( REL_VERSION_BUILD 2)

# this file is not under version control.
# Must be created with correct value for BUILD_ENV
include(platform.txt)


if (${BUILD_ENV} STREQUAL "Linux Ubuntu 22.04")
   message("Build enviromnet for Linux (1)")
   set(CMAKE_PREFIX_PATH "/media/angelo/Windows7_OS/ANGELO/bin/Qt_6.11/6.11.0/gcc_64")
   set(QT_DIR  "/media/angelo/Windows7_OS/ANGELO/bin/Qt_6.11/6.11.0/gcc_64/lib/cmake/Qt6")
   set(Qt6_DIR "/media/angelo/Windows7_OS/ANGELO/bin/Qt_6.11/6.11.0/gcc_64/lib/cmake/Qt6")
   set(QT_BIN_DIR "/media/angelo/Windows7_OS/ANGELO/bin/Qt_6.11/6.11.0/gcc_64/bin")
endif()

if (${BUILD_ENV} STREQUAL "Windows 10 Lab")
   message("Build enviromnet for Windows 10 (1)")
   set(CMAKE_PREFIX_PATH "C:/Users/necom007/Documents/tool/Qt/Qt6.11/6.11.0/msvc2022_64")
endif()


add_compile_definitions(
VERSION_MAJOR=${REL_VERSION_MAJOR}
VERSION_MINOR=${REL_VERSION_MINOR}
VERSION_BUILD=${REL_VERSION_BUILD}
VERSION_STAGE="beta"

NUMBER_OF_MEDIA_DECKS=4
)


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
   LinguistTools
   REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
   Core
   Gui
   Widgets
   OpenGL
   Network
   Multimedia
   MultimediaWidgets
   LinguistTools
   REQUIRED)


find_package(usb REQUIRED)

qt_standard_project_setup(REQUIRES 6.11)


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

set( TRANSLATIONS player_it.ts)

qt_add_translations(starvax_player
    TS_FILES ${TRANSLATIONS}
    SOURCES  ${STARVAX_SOURCES}
)

if(WIN32)
   file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}" NATIVE_SOURCE_DIR)
   file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}" NATIVE_BINARY_DIR)

   set( RELEASE_PATH ${NATIVE_SOURCE_DIR}\\..\\Starvax_${REL_VERSION_MAJOR}.${REL_VERSION_MINOR}.${REL_VERSION_BUILD})
   message (${RELEASE_PATH})

   add_custom_target( copy_deps
     COMMAND if not exist ${RELEASE_PATH} mkdir ${RELEASE_PATH}

     COMMAND xcopy /Y ${NATIVE_BINARY_DIR}\\Release\\starvax_player.exe  ${RELEASE_PATH}
     COMMAND xcopy /R /Y /E ${NATIVE_SOURCE_DIR}\\res  ${RELEASE_PATH}\\res\\
     COMMAND xcopy /R /Y /E ${NATIVE_SOURCE_DIR}\\modules\\windows\\usb\\lib\\dll\\libusb-1.0.dll  ${RELEASE_PATH}
     COMMAND ${QT_DIR}\\..\\..\\..\\bin\\windeployqt.exe --multimedia ${RELEASE_PATH}\\starvax_player.exe

     COMMAND del /F /Q ${RELEASE_PATH}\\vc_redist.x64.exe
     COMMAND rmdir /S /Q  ${RELEASE_PATH}\\translations
     )
endif()

if (UNIX)
   add_custom_target( copy_deps
     COMMAND cp -r ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}
     )
endif()

# this is executed by GitHub Actions only
if (UNIX)

   install(TARGETS starvax_player
   DESTINATION bin
   )

   # resource folder
   install(DIRECTORY res
   DESTINATION bin
   )

   # desktop file
   install(FILES deploy/linux/starvaxplayer.desktop
   DESTINATION share/applications
   )

   # application icon
   install(FILES resources/starvax.png
   DESTINATION share/icons/hicolor/256x256/apps
)
endif()
