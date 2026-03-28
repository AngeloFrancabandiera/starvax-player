if (WIN32)
   include_directories(${CMAKE_SOURCE_DIR}/modules/windows/usb/include)

   find_library(USB_LIBRARY
       NAMES libusb-1.0
       PATHS "${CMAKE_SOURCE_DIR}/modules/windows/usb/lib/dll"
       REQUIRED
   )

endif (WIN32)


if (UNIX)
  find_library(USB_LIBRARY
      NAMES usb-1.0
      REQUIRED
  )

endif(UNIX)
