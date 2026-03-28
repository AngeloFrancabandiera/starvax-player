echo

rem %1: source file directory
rem %2: build directory


set SRC_DIR_RES=%1\\res
set SRC_DIR_LIB=%1\\modules\\windows
set DEST_DIR=%2
set DEST_DIR_RES=%2\\res


if not exist "%DEST_DIR_RES%" mkdir "%DEST_DIR_RES%"

rem resource files
xcopy "%SRC_DIR_RES%"  "%DEST_DIR_RES%"   /E /Y
rem runtime DLLs
copy "%SRC_DIR_LIB%\\mdk-sdk\\lib\\x64\\*.dll"  "%DEST_DIR%"   /Y
copy "%SRC_DIR_LIB%\\usb\\lib\\dll\\*.dll"  "%DEST_DIR%"   /Y

