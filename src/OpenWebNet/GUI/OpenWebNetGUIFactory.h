#ifndef OPENWEBNETGUIFACTORY_H
#define OPENWEBNETGUIFACTORY_H

class OpenWebNetPanel;
class OwnModel;
class ApplicationSettings;
class QWidget;
class OpenWebNetEngine_IF;
class StatusDisplay;


class OpenWebNetGUIFactory
{
public:
   OpenWebNetGUIFactory();

   OpenWebNetPanel * buildPanel( QWidget * parent, OwnModel * model,
                                 OpenWebNetEngine_IF & engine,
                                 ApplicationSettings & settings,
                                 StatusDisplay & display);
};

#endif // OPENWEBNETGUIFACTORY_H
