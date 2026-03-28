echo off
REM  This script creates a folder parallel to project folder that holds:
REM  - starvax executable
REM  - QT related DLLs
REM  - USB lib DLLs
REM  - MDK library
REM  - resource files (translations, icons, ...)

REM  INPUTS: to be filled before run
REM  ---------------------------------------

REM : directory that holds starvax binary. 
set BUILD_DIR=..\..\..\build-starvax-player-Qt5_15_x64-Release\
REM : QT library folder, containing DLLs for QT modules
set QT_LIB_DIR=c:\Qt\Qt-5.15.2_x64\bin

set MDK_LIB_DIR=..\..\modules\windows\mdk-sdk\lib\x64


REM go to location of this file
cd %~dp0

REM ---------------------------
echo on
set DEST_DIR=..\..\..\starvax-deploy-x64-%date:~-4%_%date:~3,2%_%date:~0,2%
mkdir %DEST_DIR%

xcopy /E /Y /I %BUILD_DIR%\res  %DEST_DIR%\res
copy %BUILD_DIR%\libusb-1.0.dll  %DEST_DIR%
copy %BUILD_DIR%\starvax_player.exe  %DEST_DIR%

REM MDK library
copy %MDK_LIB_DIR%\*.dll   %DEST_DIR%

REM QT libraries

copy %QT_LIB_DIR%\Qt5Svg.dll   %DEST_DIR%
copy %QT_LIB_DIR%\Qt5OpenGL.dll   %DEST_DIR%
copy %QT_LIB_DIR%\Qt5Widgets.dll   %DEST_DIR%
copy %QT_LIB_DIR%\Qt5Gui.dll   %DEST_DIR%
copy %QT_LIB_DIR%\Qt5Network.dll   %DEST_DIR%
copy %QT_LIB_DIR%\Qt5Core.dll   %DEST_DIR%


mkdir %DEST_DIR%\platforms
mkdir %DEST_DIR%\imageformats

copy %QT_LIB_DIR%\..\plugins\platforms\qminimal.dll   %DEST_DIR%\platforms
copy %QT_LIB_DIR%\..\plugins\platforms\qwindows.dll   %DEST_DIR%\platforms

copy %QT_LIB_DIR%\..\plugins\imageformats\qsvg.dll    %DEST_DIR%\imageformats
copy %QT_LIB_DIR%\..\plugins\imageformats\qjpeg.dll    %DEST_DIR%\imageformats

pause 