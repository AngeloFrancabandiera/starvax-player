#ifndef _APPLICATION_ICON
#define _APPLICATION_ICON

/** @file ApplicationIcon.h
  * This file is a utlity to exploit icons in
  * pre-defined folder
  */

#include "qapplication.h"
#include "qicon.h"

/** folder where icons are stored */
#define RES_IMAGES_SUBFOLDER    QString(qApp->applicationDirPath() + "/res/images/")
#define FONT_IMAGES_SUBFOLDER   QString(qApp->applicationDirPath() + "/res/fonts/")
#define STYLE_SUBFOLDER         QString(qApp->applicationDirPath() + "/res/styles/")

/** utility to build complete path from icon file name */
#define ApplicationIcon(file)    QIcon( RES_IMAGES_SUBFOLDER + file)

/** to be used only for QPixmap */
#define IconPath(file)        QString( RES_IMAGES_SUBFOLDER + file )
#define FontPath(file)        QString( FONT_IMAGES_SUBFOLDER + file )

#endif /* _APPLICATION_ICON */
