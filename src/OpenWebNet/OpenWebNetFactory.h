#ifndef OPENWEBNETFACTORY_H
#define OPENWEBNETFACTORY_H


#include <QList>

class QAction;
class OpenWebNetEngine_IF;
class ApplicationSettings;
class StatusDisplay;
class OwnModel;


/**
 * @brief Factory for engine/GUI of Open Web Net functionality
 */
class OpenWebNetFactory
{
public:
   OpenWebNetFactory();

   OwnModel * buildModel();

   OpenWebNetEngine_IF * buildEngine( const ApplicationSettings & settings,
                                      StatusDisplay * msgDisplay);
};

#endif // OPENWEBNETFACTORY_H
