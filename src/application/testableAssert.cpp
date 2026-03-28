#include "testableAssert.h"
#include <QMessageBox>

namespace Application {

void Assert(bool c, const char * file, int line)
{
   if (c == false)
   {
      /* report error and quit application */
      QMessageBox::warning( nullptr, QObject::tr("Fatal error"),
                            QObject::tr("The application will be terminated"
                                        " for an error in file: \n"
                                        "%1   line %2").arg(file).arg(line));

      exit(1);
   }
}

void Assert(bool c, const QString & msg)
{
   if (c == false)
   {
      /* report error and quit application */
      QMessageBox::warning( nullptr, QObject::tr("Fatal error"),
                            QObject::tr("The application will be terminated"
                                        " due to the following fatal error: \n"
                                        "%1").arg(msg));

      exit(1);
   }
}

} // namespace Application
