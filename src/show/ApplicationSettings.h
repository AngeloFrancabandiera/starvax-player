#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>
#include <QList>
#include <QString>

class QMainWindow;

class ApplicationSettings : public QObject
{
   Q_OBJECT
public:
   explicit ApplicationSettings(QObject *parent = nullptr) :
      QObject(parent) {}

public:
   typedef enum
   {
      NATIVE = 0,
      LOCALE
   } Language;

   struct OpenWebNetSettings
   {
      QString serverIpAddress;
      QString configFilePath; /**< full path of OWN configuration file */
   };

signals:
   /** emitted when the list of recent shows changes */
   void recentShowsChanged();

   /** emitted when default volume is changed for line A*/
   void defaultVolumeChangedLineA(int);
   /** emitted when default volume is changed for line B*/
   void defaultVolumeChangedLineB(int);
   /** emitted when Open Web Net configuration file changes */
   void OWN_configFileChanged( const QString & configFile) const;
   /** emitted when tetx editor font size is changed */
   void textEditorFontSizeChanged( int ptSize) const;
   /** emitted when tetx editor font size is changed */
   void panelFontSizeChanged( int ptSize) const;
   /** emitted when icon size changes */
   void iconSizeChanged( int ptSize) const;
   /** emitted when playback forward or backward step is set */
   void playbackStepChanged( int stepMs) const;
   /** emitted when save flag is changed */
   void saveScriptFontFlagChanged( bool save) const;
   /** emitted when 'activate after playback' behaviour is changed  */
   void activateNextAfterPLaybackChanged( bool activateNext) const;

public:
   /** @{ full path of last opened show*/
   void addRecentShowFile(const QString &filePath);
   void removeRecentShowFile(const QString &filePath);
   QStringList recentShowFiles() const;
   int maxRecentShowFiles() const;
   /** @} */

   /** @{ volume that is set when 'default' button is pressed */
   void setDefaultVolumeLineA(int volume);
   int defaultVolumeLineA() const;
   void setDefaultVolumeLineB(int volume);
   int defaultVolumeLineB() const;
   /** @} */

   /** @{ current language */
   void setLanguage(Language language);
   Language language() const;
   /** @} */

   /** @{ position of main window and docked widgets */
   void saveWindowSetting(const QByteArray windowSetting);
   void saveWindowGeometry(const QByteArray windowGeometry);
   QByteArray getPreviousWindowSetting() const;
   QByteArray getPreviousWindowGeometry() const;
   /** @} */

   /** @{ last directory for opening files */
   void setOpenMusicFolder( int line, const QString & folder);
   QString openMusicFolder( int line) const;
   void setOpenScriptFolder( const QString & folder);
   QString openScriptFolder() const;
   /** @} */

   /** @{ password to control player from remote */
   QString remotePassword() const;
   void setRemotePassword( const QString & password);
   /** @} */

   /** @{ number of channels for light preset */
   void setNumberOfDmxChannels( int numbOfChannels);
   int numberOfDmxChannels() const;
   /** @} */

   /** @{ settings for Open Web Net. Used to control hall lights */
   void storeOpenWebNetSettings(const OpenWebNetSettings & guiSettings) const;
   OpenWebNetSettings getOpenWebNetSettings() const;
   /** @}*/

   /** @{ font for text editor and panels */
   void storeTextEditorFontSize( int pointSize) const;
   int getTextEditorFontSize() const;
   void storeSidePanelsFontSize( int pointSize) const;
   int getSidePanelsFontSize() const;
   /** @}*/

   /** @{ size for GUI icons in toolbars */
   void storeIconSize( int pointSize) const;
   int getIconSize() const;
   /** @}*/

   /** @{ duration of a forward or backward stop (milliseconds) */
   void setPlaybackStep(int stepMs);
   int playbackStep() const;
   /** @} */

   /** @{ flag to preserve info on script font size  */
   void setSaveFontSize(bool save);
   bool getSaveFontSize() const;
   /** @} */

   /** @{ after playback, switch to next track or rewind current */
   void setActivateNextAfterPLayback( bool activateNext);
   bool activateNextAfterPLayback() const;
   /** @} */

public slots:
   /** @{ list of DMX channels set as "Manual only" */
   void manualOnlyChannelChanged( bool manual, int channel);
   QList<int> manualOnlyChannels() const;
   /** @} */
};

#endif // APPLICATIONSETTINGS_H
