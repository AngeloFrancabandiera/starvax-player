#include "ApplicationSettings.h"
#include <QSettings>
#include <QApplication>


/** @class ApplicationSettings
 *
 * @brief This class holds all settings to be reloaded on next program launch.
 */

const QString COMPANY_TAG = QString("Me.Te.Or.");
const QString PRODUCT_TAG = QString("Starvax Player");
const int NUMBER_OF_RECENT_SHOW_FILES = 5;
const int  DEFAULT_NUM_OF_DMX_CHANNELS = 12;
const int  DEFAULT_FONT_SIZE = 11;
const int  DEFAULT_ICON_SIZE = 24;


void ApplicationSettings::addRecentShowFile(const QString &filePath)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);

   /* load list of last shows and remove the older, if needed */
   QStringList fileSet = setting.value( "recent show files", QStringList()).toStringList();

   fileSet.removeAll(filePath);
   fileSet.prepend(filePath);

   while (fileSet.size() >= NUMBER_OF_RECENT_SHOW_FILES)
   {
      /* remove the oldest inserted file  */
      fileSet.removeLast();
   }

   setting.setValue( "recent show files", fileSet);
   emit recentShowsChanged();
}

void ApplicationSettings::removeRecentShowFile(const QString & filePath)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);

   /* load list of last shows and remove the older, if needed */
   QStringList fileSet = setting.value( "recent show files", QStringList()).toStringList();

   fileSet.removeAll(filePath);

   setting.setValue( "recent show files", fileSet);
   emit recentShowsChanged();
}

QStringList ApplicationSettings::recentShowFiles() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QStringList fileSet = setting.value( "recent show files", QStringList()).toStringList();

   return fileSet;
}

int ApplicationSettings::maxRecentShowFiles() const
{
   return NUMBER_OF_RECENT_SHOW_FILES;
}

void ApplicationSettings::setDefaultVolumeLineA(int volume)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( "default volume line A", volume);

   emit defaultVolumeChangedLineA( volume);
}

int ApplicationSettings::defaultVolumeLineA() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   int defVolume = setting.value( "default volume line A", 100).toInt();

   return defVolume;
}

void ApplicationSettings::setDefaultVolumeLineB(int volume)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( "default volume line B", volume);

   emit defaultVolumeChangedLineB( volume);
}

int ApplicationSettings::defaultVolumeLineB() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   int defVolume = setting.value( "default volume line B", 100).toInt();

   return defVolume;
}

void ApplicationSettings::setLanguage(ApplicationSettings::Language language)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( "language", language);
}

ApplicationSettings::Language ApplicationSettings::language() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   Language language = static_cast<Language>(setting.value( "language", NATIVE).toInt());

   return language;
}

void ApplicationSettings::saveWindowSetting(const QByteArray windowSetting)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( "window state", windowSetting);
}

QByteArray ApplicationSettings::getPreviousWindowSetting() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QByteArray windowSetting = setting.value( "window state").toByteArray();

   return windowSetting;
}

void ApplicationSettings::saveWindowGeometry(const QByteArray windowGeometry)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( "window geometry", windowGeometry);
}

QByteArray ApplicationSettings::getPreviousWindowGeometry() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QByteArray windowGeometry = setting.value( "window geometry").toByteArray();

   return windowGeometry;
}


void ApplicationSettings::setOpenMusicFolder( int line, const QString & folder)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   if (line == 0) {
      setting.setValue( "open music folder LA", folder);
   }
   else {
      setting.setValue( "open music folder LB", folder);
   }
}

QString ApplicationSettings::openMusicFolder(int line) const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QString musicFolder;

   if (line == 0) {
      musicFolder = setting.value( "open music folder LA").toString();
   }
   else {
      musicFolder = setting.value( "open music folder LB").toString();
   }

   return musicFolder;
}

void ApplicationSettings::setOpenScriptFolder(const QString & folder)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( "open script folder", folder);
}

QString ApplicationSettings::openScriptFolder() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QString musicFolder = setting.value( "open script folder").toString();

   return musicFolder;
}

QString ApplicationSettings::remotePassword() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QString password = setting.value( "remote password").toString();

   return password;
}

void ApplicationSettings::setRemotePassword(const QString & password)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( "remote password", password);
}

void ApplicationSettings::setNumberOfDmxChannels(int numbOfChannels)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( "number of DMX channels", numbOfChannels);
}

int ApplicationSettings::numberOfDmxChannels() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   int numChannels = setting.value( "number of DMX channels",
                                    DEFAULT_NUM_OF_DMX_CHANNELS).toInt();

   return numChannels;
}

void ApplicationSettings::storeOpenWebNetSettings(
      const ApplicationSettings::OpenWebNetSettings & guiSettings) const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QString oldPath = setting.value( "OWN config path", "").toString();

   setting.setValue( "OWN server IP addr", guiSettings.serverIpAddress);
   setting.setValue( "OWN config path", guiSettings.configFilePath);

   if (oldPath != guiSettings.configFilePath)
   {
      emit OWN_configFileChanged( guiSettings.configFilePath);
   }
}

ApplicationSettings::OpenWebNetSettings
ApplicationSettings::getOpenWebNetSettings() const
{
   static const QString defaultLightFilePath =
         QString("%1/res/OWN_plants/default.light").arg( qApp->applicationDirPath());

   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   ApplicationSettings::OpenWebNetSettings own;
   own.serverIpAddress = setting.value( "OWN server IP addr", "0.0.0.0").toString();
   own.configFilePath = setting.value( "OWN config path", defaultLightFilePath).toString();

   return own;
}

void ApplicationSettings::storeTextEditorFontSize(int pointSize) const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);

   int oldSize = setting.value( "editor point size", DEFAULT_FONT_SIZE).toInt();
   setting.setValue( "editor point size", pointSize);

   if (oldSize != pointSize)
   {
      emit textEditorFontSizeChanged( pointSize);
   }
}

int ApplicationSettings::getTextEditorFontSize() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   int fontSize = setting.value( "editor point size", DEFAULT_FONT_SIZE).toInt();

   return fontSize;
}

void ApplicationSettings::storeSidePanelsFontSize(int pointSize) const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);

   int oldSize = setting.value( "panel point size", DEFAULT_FONT_SIZE).toInt();
   setting.setValue( "panel point size", pointSize);

   if (oldSize != pointSize)
   {
      emit panelFontSizeChanged( pointSize);
   }
}

int ApplicationSettings::getSidePanelsFontSize() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   int fontSize = setting.value( "panel point size", DEFAULT_FONT_SIZE).toInt();

   return fontSize;
}

void ApplicationSettings::storeIconSize(int pointSize) const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);

   int oldSize = setting.value( "icon size", DEFAULT_ICON_SIZE).toInt();
   setting.setValue( "icon size", pointSize);

   if (oldSize != pointSize)
   {
      emit iconSizeChanged( pointSize);
   }
}

int ApplicationSettings::getIconSize() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   int iconSize = setting.value( "icon size", DEFAULT_ICON_SIZE).toInt();

   return iconSize;
}

void ApplicationSettings::setPlaybackStep(int stepMs)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);

   int oldStep = setting.value( "media playback step", DEFAULT_FONT_SIZE).toInt();

   if (oldStep != stepMs)
   {
      setting.setValue( "media playback step", stepMs);
      emit playbackStepChanged( stepMs);
   }
}

int ApplicationSettings::playbackStep() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   int renderModeIndex = setting.value( "media playback step", 1000).toInt();

   return renderModeIndex;
}

void ApplicationSettings::setSaveFontSize( bool save)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);

   bool oldFlag = setting.value( "save script font size", false).toBool();

   if (oldFlag != save)
   {
      setting.setValue( "save script font size", save);
      emit saveScriptFontFlagChanged( save);
   }
}

bool ApplicationSettings::getSaveFontSize() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   bool saveFlag = setting.value( "save script font size", false).toBool();

   return saveFlag;
}

void ApplicationSettings::setActivateNextAfterPLayback(bool activateNext)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);

   bool oldFlag = setting.value( "activate next after playback", true).toBool();

   if (oldFlag != activateNext)
   {
      setting.setValue( "activate next after playback", activateNext);
      emit activateNextAfterPLaybackChanged( activateNext);
   }
}

bool ApplicationSettings::activateNextAfterPLayback() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   bool flag = setting.value( "activate next after playback", true).toBool();

   return flag;
}


void ApplicationSettings::manualOnlyChannelChanged( bool manual, int channel)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QList<QVariant> manualOnlyChannels = setting.value("manual only DMX", QList<QVariant>()).toList();

   manualOnlyChannels.removeAll( QVariant(channel));

   if (manual)
   {
      /* channel is present only once */
      manualOnlyChannels.append( QVariant(channel));
   }

   setting.setValue( "manual only DMX", QVariant(manualOnlyChannels));
}

QList<int> ApplicationSettings::manualOnlyChannels() const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QList<QVariant> settingCahnnels = setting.value("manual only DMX", QList<QVariant>()).toList();

   QList<int> manualChannels;

   foreach( QVariant item, settingCahnnels)
   {
      if (item.canConvert<int>())
      {
         manualChannels << item.toInt();
      }
   }

   return manualChannels;
}
