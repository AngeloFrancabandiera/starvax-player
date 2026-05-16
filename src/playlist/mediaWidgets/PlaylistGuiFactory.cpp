#include <QBoxLayout>
#include <QCommonStyle>
#include <QAction>
#include <QCheckBox>
#include <QDockWidget>
#include <QStringList>

#include "PlaylistGuiFactory.h"
#include "PlaylistDecks.h"
#include "IF_MediaEngineInterface.h"
#include "mediaPositionSlider.h"
#include "MediaAutomation.h"
#include "MediaListView.h"
#include "MediaListModel.h"
#include "volumeslider.h"
#include "ActionListController.h"
#include "playlistPainter.h"
#include "AudioVideoPlayBar.h"
#include "PicturePlaybar.h"
#include "PlaylistBar.h"
#include "ApplicationIcon.h"
#include "ApplicationSettings.h"
#include "QSpinBox"
#include "FileInport.h"
#include "testableAssert.h"
#include "ActionListView.h"
#include "FullScreenMediaWidget.h"
#include "ApplicationIcon.h"



PlaylistGuiFactory::PlaylistGuiFactory( const ApplicationSettings & settings,
                                        Playlist::Deck deck,
                                        IF_MediaEngineInterface *engine,
                                        MediaListModel * mediaList,
                                        FileInport *fileInport,
                                        QObject *parent) :
   QObject(parent),
   m_settings(settings),
   m_deck(deck),
   m_engine( engine),
   m_mediaList( mediaList),
   m_fileInport(fileInport)
{
   T_ASSERT( engine != nullptr);
   T_ASSERT( mediaList != nullptr);
}

MediaPositionSlider *PlaylistGuiFactory::buildSeekbar( QWidget *parent)
{
   MediaPositionSlider *positionSlider = new MediaPositionSlider( parent);

   connect( m_engine, SIGNAL(totalTimeChanged(qint64)), positionSlider, SLOT(setDurationMs(qint64)) );
   connect( m_engine, SIGNAL(tick(qint64)), positionSlider, SLOT(setPositionMs(qint64)) );

   connect( positionSlider, SIGNAL(positionChangeRequested(qint64)),
            m_engine, SLOT(onUserPositionRequested(qint64)) );

   return positionSlider;
}

VolumeSlider *PlaylistGuiFactory::buildVolumeBar(QBoxLayout *container)
{
   Qt::Orientation orientation;
   orientation = (container->direction() <= QBoxLayout::RightToLeft) ?
                    Qt::Horizontal : Qt::Vertical;

   VolumeSlider *volumeSlider = new VolumeSlider( *m_engine, orientation, container);
   return volumeSlider;
}

AudioVideoPlayBar *PlaylistGuiFactory::buildAudioVideoPlaybar( QWidget *parent)
{
   MediaPositionSlider *positionSlider = buildSeekbar( parent);
   T_ASSERT( positionSlider != nullptr);
   AudioVideoPlayBar *playbar = new AudioVideoPlayBar( *m_engine, *positionSlider, parent);

   connect( m_engine, & IF_MediaEngineInterface::audioOnlyChanged,
            playbar, & AudioVideoPlayBar::onAudioOnlyChanged );

   return playbar;
}

PicturePlaybar *PlaylistGuiFactory::buildPicturePlaybar( MediaAutomation & automation,
                                                         QWidget * parent)
{
   return new PicturePlaybar( m_engine, automation, parent);
}

PlaylistBar * PlaylistGuiFactory::buildPlaylistBar( AudioVideoPlayBar * audioVideoBar,
                                                    PicturePlaybar * pictureBar,

                                                    QWidget * parent)
{
   T_ASSERT (audioVideoBar != nullptr);
   T_ASSERT (pictureBar != nullptr);
   return new PlaylistBar( *audioVideoBar, *pictureBar, *m_mediaList, parent);
}

ActionListView *PlaylistGuiFactory::buildPlaylistView( PlaylistBar *playbar,
                                                       ActionListController *actionController,
                                                       StatusDisplay * msgDisplay,
                                                       QWidget *parent)
{
   T_ASSERT( msgDisplay);

   PlaylistPainter * painter = new PlaylistPainter( m_settings);
   ActionListView * playlistView = new MediaListView( *actionController, painter,
                                                      *msgDisplay, parent);

   playlistView->setModel( m_mediaList);
   playlistView->setActionBar( playbar);

   connect( & m_settings, SIGNAL(panelFontSizeChanged(int)),
            playlistView, SLOT(onPanelFontSizeChanged(int)) );

   connect( playlistView, &ActionListView::redoRequest, m_mediaList, &MediaListModel::redo);
   connect( playlistView, &ActionListView::undoRequest, m_mediaList, &MediaListModel::undo);

   return playlistView;
}


void PlaylistGuiFactory::buildPlaylistPanel( MediaAutomation *automation,
                                             ActionListView * playlistView,
                                             QAction * setEditModeAction,
                                             QDockWidget *container)
{

   QVBoxLayout *controlLayout = nullptr;
   QVBoxLayout *volumeLayout = nullptr;
   buildInternalLayouts( container, &controlLayout, &volumeLayout);

   buildPlaylistControlArea( automation,
                             playlistView,
                             setEditModeAction,
                             controlLayout);
   buildVolumeBar( volumeLayout);
}


void PlaylistGuiFactory::buildInternalLayouts( QDockWidget *container,
                                               QVBoxLayout **mediaListLayout,
                                               QVBoxLayout **volumeLayout)
{
   QWidget * dockPlaylist = new QWidget( container);
   container->setWidget( dockPlaylist);

   QHBoxLayout *panelLayout = new QHBoxLayout( dockPlaylist);

   QWidget * playlistContainer = new QWidget( dockPlaylist);
   playlistContainer->setObjectName("playlistContainer");
   *mediaListLayout = new QVBoxLayout(playlistContainer);
   (*mediaListLayout)->setContentsMargins(2,2,2,2);
   (*mediaListLayout)->setObjectName("m_playlistLayout");

   QWidget * volumeContainer = new QWidget( dockPlaylist);
   volumeContainer->setObjectName("volumeContainer");
   *volumeLayout = new QVBoxLayout( volumeContainer);
   (*volumeLayout)->setContentsMargins(2,2,2,2);
   (*volumeLayout)->setObjectName("m_volumeLayout");

   panelLayout->addWidget( volumeContainer);
   panelLayout->addWidget( playlistContainer);
}

void PlaylistGuiFactory::buildPlaylistControlArea( MediaAutomation *automation,
                                                   ActionListView * playlistView,
                                                   QAction * setEditModeAction,
                                                   QLayout *container)
{
   QCommonStyle style;
   container->setContentsMargins( 2,2,2,2);
   QSize iconSize = QSize(m_settings.getIconSize(), m_settings.getIconSize());

   T_ASSERT( m_deck < NUMBER_OF_MEDIA_DECKS);

   playlistView->setObjectName(QString("playlist_%1").arg(Playlist::toLetter(m_deck)));

   QWidget *toolbar = new QWidget( container->parentWidget());
   toolbar->setLayout( new QHBoxLayout());
   toolbar->layout()->setContentsMargins( 2,2,2,2);

   MediaPositionSlider *seekBar = buildSeekbar( container->parentWidget());
   container->addWidget( seekBar);

   /* 'fader' button */
   QPushButton *faderButton = new QPushButton( QIcon(IconPath("fade_out.png")),
                                               QString(), toolbar);
   faderButton->setToolTip( tr("start/stop volume fading"));
   faderButton->setFocusPolicy(Qt::NoFocus);
   connect( faderButton, SIGNAL(clicked()), automation, SLOT(toggleFader()) );
   toolbar->layout()->addWidget( faderButton);
   faderButton->setMaximumSize(iconSize);

   /* 'default voume' button */
   QPushButton *defaultVolumeButton = new QPushButton( QIcon(IconPath("def_vol.png")),
                                                       QString(), toolbar);
   defaultVolumeButton->setToolTip( tr("apply default volume"));
   defaultVolumeButton->setFocusPolicy(Qt::NoFocus);
   connect( defaultVolumeButton, SIGNAL(clicked()), automation, SLOT(applyDefaultVolume()) );
   toolbar->layout()->addWidget( defaultVolumeButton);
   defaultVolumeButton->setMaximumSize(iconSize);

   QPushButton *deactivateButton = new QPushButton( QIcon(IconPath("close.png")),
                                                    QString(), toolbar);
   deactivateButton->setToolTip(tr("activate nothing"));
   connect( deactivateButton, & QPushButton::clicked, playlistView, & ActionListView::deActivate);
   toolbar->layout()->addWidget( deactivateButton);
   deactivateButton->setMaximumSize(iconSize);

   QCheckBox *onTopButton = new QCheckBox(QString(), toolbar);
   onTopButton->setToolTip(tr("video on top"));
   onTopButton->setStyleSheet(
                          "QCheckBox::indicator {"
                          "width: 26px;"
                          "height: 26px; }"
                          "QCheckBox::indicator:unchecked {"
                          "    image: url( :/images/HardCodedIcons/onTop_off.png); }"
                          "QCheckBox::indicator:checked {"
                          "    image: url( :/images/HardCodedIcons/onTop_on.png); }" );
   onTopButton->setCheckable( true);
   onTopButton->setChecked( false);
   /* UI operation on button */
   connect( onTopButton, & QPushButton::clicked, m_engine, & IF_MediaEngineInterface::showOnTop);
   /* 'onTop' changed by automation */
   connect( m_engine, & IF_MediaEngineInterface::onTopChanged, onTopButton, & QPushButton::setChecked);
   toolbar->layout()->addWidget( onTopButton);
   onTopButton->setMaximumSize(iconSize);

   toolbar->layout()->addItem( new QSpacerItem( 1,1, QSizePolicy::Expanding));

   /* 'open media' button */
   QPushButton *openMediaButton = new QPushButton( QIcon(IconPath("open_music.png")),
                                                 QString(), toolbar);
   openMediaButton->setToolTip( tr("open a dialog to load new media files"));
   openMediaButton->setFocusPolicy(Qt::NoFocus);
   connectOpenMediaFunction( openMediaButton);
   toolbar->layout()->addWidget( openMediaButton);
   openMediaButton->setMaximumSize(iconSize);

   /* 'shift up' button */
   QPushButton *shiftUpButton = new QPushButton( style.standardIcon(QStyle::SP_ArrowUp),
                                                 QString(), toolbar);
   shiftUpButton->setToolTip( tr("bring focused element one position above"));
   shiftUpButton->setFocusPolicy(Qt::NoFocus);
   connect( shiftUpButton, SIGNAL(clicked()), playlistView, SLOT(shiftFocusedUp()) );
   toolbar->layout()->addWidget( shiftUpButton);
   shiftUpButton->setMaximumSize(iconSize);

   /* 'shift down' button */
   QPushButton *shiftDownButton = new QPushButton( style.standardIcon(QStyle::SP_ArrowDown),
                                                   QString(), toolbar);
   shiftDownButton->setToolTip( tr("bring focused element one position below"));
   shiftDownButton->setFocusPolicy(Qt::NoFocus);
   connect( shiftDownButton, SIGNAL(clicked()), playlistView, SLOT(shiftFocusedDown()) );
   toolbar->layout()->addWidget( shiftDownButton);
   shiftDownButton->setMaximumSize(iconSize);

   /* 'delete selected' button */
   QPushButton *deleteButton = new QPushButton( style.standardIcon(QStyle::SP_TrashIcon),
                                                   QString(), toolbar);
   deleteButton->setToolTip( tr("delete selected items"));
   deleteButton->setFocusPolicy(Qt::NoFocus);
   connect( deleteButton, SIGNAL(clicked()), playlistView, SLOT(requestToDeleteSelection()) );
   toolbar->layout()->addWidget( deleteButton);
   deleteButton->setMaximumSize(iconSize);

   /* undo/redo buttons */
   QPushButton *undoButton = new QPushButton( QIcon(IconPath("undo.png")), QString(), toolbar);
   undoButton->setToolTip( tr("undo"));
   undoButton->setFocusPolicy(Qt::NoFocus);
   undoButton->setEnabled( false);
   connect( undoButton, & QPushButton::clicked, m_mediaList, & MediaListModel::undo );
   connect( m_mediaList, & MediaListModel::canUndoChanged, undoButton, & QPushButton::setEnabled);
   toolbar->layout()->addWidget( undoButton);
   undoButton->setMaximumSize(iconSize);

   QPushButton *redoButton = new QPushButton( QIcon(IconPath("redo.png")), QString(), toolbar);
   redoButton->setToolTip( tr("redo"));
   redoButton->setFocusPolicy(Qt::NoFocus);
   redoButton->setEnabled( false);
   connect( redoButton, & QPushButton::clicked, m_mediaList, & MediaListModel::redo );
   connect( m_mediaList, & MediaListModel::canRedoChanged, redoButton, & QPushButton::setEnabled);
   toolbar->layout()->addWidget( redoButton);
   redoButton->setMaximumSize(iconSize);

   container->addWidget( toolbar);
   container->addWidget( playlistView);

   /* bind editing actions to edit mode */
   connect( setEditModeAction, & QAction::triggered, openMediaButton, & QWidget::setVisible );
   connect( setEditModeAction, & QAction::triggered, shiftUpButton, & QWidget::setVisible );
   connect( setEditModeAction, & QAction::triggered, shiftDownButton, & QWidget::setVisible );
   connect( setEditModeAction, & QAction::triggered, deleteButton, & QWidget::setVisible );
   connect( setEditModeAction, & QAction::triggered, undoButton, & QWidget::setVisible );
   connect( setEditModeAction, & QAction::triggered, redoButton, & QWidget::setVisible );

   connect( setEditModeAction, SIGNAL(triggered(bool)), playlistView, SLOT(setEditMode(bool)));
}

void PlaylistGuiFactory::connectOpenMediaFunction( QPushButton *openMediaButton)
{
   connect( openMediaButton, & QPushButton::clicked, this, & PlaylistGuiFactory::onOpenMediaClicked);
}


void PlaylistGuiFactory::onOpenMediaClicked()
{
   QStringList tracks = m_fileInport->selectTracksForDeck( m_deck);

   if (tracks.length() > 0)
   {
      m_mediaList->addMediaFiles( tracks);
   }
}
