#include "OpenWebNetGUIFactory.h"
#include "OpenWebNetPanel.h"
#include <QLayout>

OpenWebNetGUIFactory::OpenWebNetGUIFactory()
{

}

OpenWebNetPanel *OpenWebNetGUIFactory::buildPanel( QWidget * parent,
                                                   OwnModel * model,
                                                   OpenWebNetEngine_IF & engine,
                                                   ApplicationSettings & settings,
                                                   StatusDisplay & display)
{
   OpenWebNetPanel * panel = new OpenWebNetPanel( parent, model, engine,
                                                  settings, display);

   parent->layout()->setContentsMargins( 1,1,1,1);
   parent->layout()->addWidget( panel);
   return panel;
}
