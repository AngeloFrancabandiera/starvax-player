#ifndef _APPLICATION_ICON
#define _APPLICATION_ICON

/** @file ApplicationIcon.h
  * This file is a utlity to exploit icons in
  * pre-defined folder
  */

#include <QIcon>
#include "ApplicationSettings.h"

/** folder where icons are stored */
#define RES_IMAGES_SUBFOLDER    (ApplicationSettings::applicationResourcePath() + "/images/")
#define FONT_IMAGES_SUBFOLDER   (ApplicationSettings::applicationResourcePath() + "/fonts/")
#define STYLE_SUBFOLDER         (ApplicationSettings::applicationResourcePath() + "/styles/")

/** utility to build complete path from icon file name */
#define ApplicationIcon(file)    QIcon( RES_IMAGES_SUBFOLDER + file)

/** to be used only for QPixmap */
#define IconPath(file)        (RES_IMAGES_SUBFOLDER + file)
#define FontPath(file)        (FONT_IMAGES_SUBFOLDER + file)

#endif /* _APPLICATION_ICON */
