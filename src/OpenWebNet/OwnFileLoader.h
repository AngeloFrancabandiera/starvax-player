#ifndef OWNFILELOADER_H
#define OWNFILELOADER_H

#include <QObject>

class OwnModel;

/**
 * @brief The OwnFileLoader class listens for events
 * from settings (or application start).
 * A parser is instantiated to load OWN items to the model.
 */
class OwnFileLoader : public QObject
{
   Q_OBJECT
public:
   explicit OwnFileLoader( OwnModel & ownModel,
                           QObject *parent = nullptr);

public slots:
   /** to be called when user has changed OWN configuration
    * file in application settings and on application start-up.
    * Configuration is loaded into model */
   void loadConfigurationFile( const QString & fullPath);

signals:
   /** finished to load configuration */
   void configurationLoaded();

private:
   OwnModel & m_ownModel;
};

#endif // OWNFILELOADER_H
