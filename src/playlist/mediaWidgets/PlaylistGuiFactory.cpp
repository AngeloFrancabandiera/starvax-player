#include <QBoxLayout>
#include <QCommonStyle>
#include <QAction>
#include <QCheckBox>

#include "PlaylistGuiFactory.h"
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
                                        QObject *parent) :
   QObject(parent),
   m_settings(settings)
{
}

MediaPositionSlider *PlaylistGuiFactory::buildSeekbar(IF_MediaEngineInterface *engine, QWidget *parent)
{
   T_ASSERT( engine != nullptr);

   MediaPositionSlider *positionSlider = new MediaPositionSlider( parent);

   connect( engine, SIGNAL(totalTimeChanged(qint64)), positionSlider, SLOT(setDurationMs(qint64)) );
   connect( engine, SIGNAL(tick(qint64)), positionSlider, SLOT(setPositionMs(qint64)) );

   connect( positionSlider, SIGNAL(positionChangeRequested(qint64)),
            engine, SLOT(onUserPositionRequested(qint64)) );

   return positionSlider;
}

VolumeSlider *PlaylistGuiFactory::buildVolumeBar(IF_MediaEngineInterface *engine,
                                                 QBoxLayout *container)
{
   T_ASSERT( engine != nullptr);

   Qt::Orientation orientation;
   orientation = (container->direction() <= QBoxLayout::RightToLeft) ?
                    Qt::Horizontal : Qt::Vertical;

   VolumeSlider *volumeSlider = new VolumeSlider( *engine, orientation, container);
   return volumeSlider;
}

AudioVideoPlayBar *PlaylistGuiFactory::buildAudioVideoPlaybar( IF_MediaEngineInterface *engine,
                                                               QWidget *parent)
{
   T_ASSERT( engine != nullptr);

   MediaPositionSlider *positionSlider = buildSeekbar( engine, parent);
   T_ASSERT( positionSlider != nullptr);
   AudioVideoPlayBar *playbar = new AudioVideoPlayBar( *engine, *positionSlider, parent);

   connect( engine, & IF_MediaEngineInterface::audioOnlyChanged,
            playbar, & AudioVideoPlayBar::onAudioOnlyChanged );

   return playbar;
}

PicturePlaybar *PlaylistGuiFactory::buildPicturePlaybar( IF_MediaEngineInterface * engine,
                                                         MediaAutomation & automation,
                                                         QWidget * parent)
{
   return new PicturePlaybar( engine, automation, parent);
}

PlaylistBar * PlaylistGuiFactory::buildPlaylistBar( AudioVideoPlayBar * audioVideoBar,
                                                    PicturePlaybar * pictureBar,
                                                    MediaListModel * mediaList,
                                                    QWidget * parent)
{
   T_ASSERT (audioVideoBar != nullptr);
   T_ASSERT (pictureBar != nullptr);
   T_ASSERT (mediaList != nullptr);
   return new PlaylistBar( *audioVideoBar, *pictureBar, *mediaList, parent);
}

ActionListView *PlaylistGuiFactory::buildPlaylistView( PlaylistBar *playbar,
                                                       MediaListModel *model,
                                                       ActionListController *actionController,
                                                       StatusDisplay * msgDisplay,
                                                       QWidget *parent)
{
   T_ASSERT( msgDisplay);

   PlaylistPainter * painter = new PlaylistPainter( m_settings);
   ActionListView * playlistView = new MediaListView( *actionController, painter,
                                                      *msgDisplay, parent);

   playlistView->setModel( model);
   playlistView->setActionBar( playbar);

   connect( & m_settings, SIGNAL(panelFontSizeChanged(int)),
            playlistView, SLOT(onPanelFontSizeChanged(int)) );

   connect( playlistView, &ActionListView::redoRequest, model, &MediaListModel::redo);
   connect( playlistView, &ActionListView::undoRequest, model, &MediaListModel::undo);

   return playlistView;
}


void PlaylistGuiFactory::buildPlaylistPanel( IF_MediaEngineInterface *engine,
                                             MediaAutomation *automation,
                                             MediaListModel *mediaModel,
                                             Playlist::Line line,
                                             ActionListView * playlistView,
                                             FileInport *fileInport,
                                             QAction * setEditModeAction,
                                             QLayout *container)
{
   QCommonStyle style;
   container->setContentsMargins( 2,2,2,2);
   QSize iconSize = QSize(m_settings.getIconSize(), m_settings.getIconSize());

   switch (line)
   {
   case Playlist::LINE_A : playlistView->setObjectName("playlist_A"); break;
   case Playlist::LINE_B : playlistView->setObjectName("playlist_B"); break;
   default: T_ASSERT(false); break;
   }

   QWidget *toolbar = new QWidget( container->parentWidget());
   toolbar->setLayout( new QHBoxLayout());
   toolbar->layout()->setContentsMargins( 2,2,2,2);

   MediaPositionSlider *seekBar = buildSeekbar( engine, container->parentWidget());
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
   connect( onTopButton, & QPushButton::clicked, engine, & IF_MediaEngineInterface::showOnTop);
   /* 'onTop' changed by automation */
   connect( engine, & IF_MediaEngineInterface::onTopChanged, onTopButton, & QPushButton::setChecked);
   toolbar->layout()->addWidget( onTopButton);
   onTopButton->setMaximumSize(iconSize);

   toolbar->layout()->addItem( new QSpacerItem( 1,1, QSizePolicy::Expanding));

   /* 'open media' button */
   QPushButton *openMediaButton = new QPushButton( QIcon(IconPath("open_music.png")),
                                                 QString(), toolbar);
   openMediaButton->setToolTip( tr("open a dialog to load new media files"));
   openMediaButton->setFocusPolicy(Qt::NoFocus);
   connectOpenMediaFunction(line, fileInport, openMediaButton);
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
   connect( undoButton, & QPushButton::clicked, mediaModel, & MediaListModel::undo );
   connect( mediaModel, & MediaListModel::canUndoChanged, undoButton, & QPushButton::setEnabled);
   toolbar->layout()->addWidget( undoButton);
   undoButton->setMaximumSize(iconSize);

   QPushButton *redoButton = new QPushButton( QIcon(IconPath("redo.png")), QString(), toolbar);
   redoButton->setToolTip( tr("redo"));
   redoButton->setFocusPolicy(Qt::NoFocus);
   redoButton->setEnabled( false);
   connect( redoButton, & QPushButton::clicked, mediaModel, & MediaListModel::redo );
   connect( mediaModel, & MediaListModel::canRedoChanged, redoButton, & QPushButton::setEnabled);
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

void PlaylistGuiFactory::connectOpenMediaFunction( Playlist::Line line, FileInport *fileInport,
                                                   QPushButton *openMediaButton)
{
   if (line == Playlist::LINE_B)
   {
      connect( openMediaButton, SIGNAL(clicked()), fileInport, SLOT(openTracksDialogLineB()) );
   }
   else
   {
      connect( openMediaButton, SIGNAL(clicked()), fileInport, SLOT(openTracksDialogLineA()) );
   }
}
