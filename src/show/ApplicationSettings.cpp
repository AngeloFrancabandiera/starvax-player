#include "ApplicationSettings.h"
#include <QSettings>
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <filesystem>

#include "WindowLayout.h"
#include "PlaylistDecks.h"
#include "testableAssert.h"


/** @class ApplicationSettings
 *
 * @brief This class holds all settings to be reloaded on next program launch.
 */

const QString COMPANY_TAG = QString("starvax_player");
const QString PRODUCT_TAG = QString("Starvax Player");
const int NUMBER_OF_RECENT_SHOW_FILES = 5;
const int  DEFAULT_NUM_OF_DMX_CHANNELS = 12;
const int  DEFAULT_FONT_SIZE = 11;
const int  DEFAULT_ICON_SIZE = 24;

namespace {
   namespace fs = std::filesystem;

   void copy_folder( const std::string & dest, const std::string & source)
   {
      try {

         fs::create_directories( dest);
         fs::copy( fs::path(source), fs::path(dest),
                   fs::copy_options::recursive | fs::copy_options::overwrite_existing);
     } catch (const fs::filesystem_error &e) {
         T_ASSERT_REP( false, QString("Failed to copy '%1' into '%2' because: %3").
                                 arg(source).arg(dest).arg(e.what()));
     }
   }
}


QString ApplicationSettings::applicationResourcePath()
{
   QString configLocation = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
   QString resourceLocation = configLocation + QDir::separator() + "res";
   QDir resourceDir = QDir( resourceLocation);

   if (resourceDir.exists() == false)
   {
      /* try to copy 'res' from binary folder */
      QString binaryPath = QCoreApplication::applicationDirPath() + QDir::separator() + "res";
      if (QDir(binaryPath).exists())
      {
         copy_folder( resourceLocation.toStdString(), binaryPath.toStdString());
      }
      else
      {
         /* this happens un unix systems starting from AppImage */

         QString usr_share_path = QCoreApplication::applicationDirPath() + "/../share/starvaxplayer/res";

         if (QDir(usr_share_path).exists())
         {
            copy_folder( resourceLocation.toStdString(), usr_share_path.toStdString());
         }
         else
         {
            T_ASSERT_REP( false, "Unable to find resource folder");
         }
      }
   }
   /* else: folder has already been copied */

   return resourceLocation;
}

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

void ApplicationSettings::setDefaultVolume( int deck, int volume)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( QString("default volume line %1").arg(Playlist::toLetter(deck)), volume);

   emit defaultVolumeChanged( deck, volume);
}

int ApplicationSettings::defaultVolume( int deck) const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   int defVolume = setting.value( QString("default volume line %1").arg(Playlist::toLetter(deck)), 100).toInt();

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

   if (windowSetting.isEmpty())
   {
      WindowLayout wlayout;
      QString defaultPath = applicationResourcePath() + QDir::separator() +
                              "layout" + QDir::separator() + "basic.metlayout";
      QFile defaultFile( defaultPath);
      bool ok = defaultFile.open( QIODeviceBase::ReadOnly);

      if (ok)
      {
         QDataStream stream( &defaultFile);
         QByteArray unused;
         wlayout.load( stream,  windowSetting, unused);

         defaultFile.close();
      }
   }

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

   if (windowGeometry.isEmpty())
   {
      WindowLayout wlayout;
      QString defaultPath = applicationResourcePath() + QDir::separator() + "layout" +
                              QDir::separator() + "basic.metlayout";
      QFile defaultFile( defaultPath);
      bool ok = defaultFile.open( QIODeviceBase::ReadOnly);

      if (ok)
      {
         QDataStream stream( &defaultFile);
         QByteArray unused;
         wlayout.load( stream, unused, windowGeometry);

         defaultFile.close();
      }
   }

   return windowGeometry;
}


void ApplicationSettings::setOpenMusicFolder( int deck, const QString & folder)
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   setting.setValue( QString("open music folder deck %1").arg(Playlist::toLetter(deck)), folder);
}

QString ApplicationSettings::openMusicFolder(int deck) const
{
   QSettings setting( QSettings::IniFormat, QSettings::UserScope, COMPANY_TAG, PRODUCT_TAG);
   QString musicFolder;

   musicFolder = setting.value( QString("open music folder deck %1").arg(Playlist::toLetter(deck))).toString();

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
         QString("%1/OWN_plants/default.light").arg( applicationResourcePath());

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
