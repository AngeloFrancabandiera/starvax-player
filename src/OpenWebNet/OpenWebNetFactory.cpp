#include "OpenWebNetFactory.h"

#include "Action.h"
#include "OpenWebNetEngine.h"
#include "ApplicationIcon.h"
#include "OwnModel.h"

OpenWebNetFactory::OpenWebNetFactory()
{

}

OwnModel * OpenWebNetFactory::buildModel()
{
   return new OwnModel( nullptr);
}

OpenWebNetEngine_IF *OpenWebNetFactory::buildEngine( const ApplicationSettings & settings,
                                                     StatusDisplay * msgDisplay)
{
   OpenWebNetEngine_IF * engine = new OpenWebNetEngine( settings, msgDisplay);
   return engine;
}


