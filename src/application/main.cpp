#include <QApplication>

#include <QSplashScreen>
#include "ApplicationIcon.h"

#include "mainwindow.h"
#include "ApplicationFactory.h"


int main(int argc, char *argv[])
{
   qApp->setAttribute(Qt::AA_UseOpenGLES); //ANGLE
   QApplication a(argc, argv);

   QSplashScreen splash(QPixmap(IconPath("splash.png")));
   splash.show();
   splash.showMessage(QString("loading version %1.%2.%3 %4 ...").
                      arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD).arg(VERSION_STAGE),
                      Qt::AlignBottom);
   a.processEvents();

   ApplicationFactory factory;
   factory.mainWindow()->show();

   splash.finish( factory.mainWindow());

   factory.mainWindow()->openApplicationArguments();

   return a.exec();
}
