#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <QString>
#include <PlaylistConstants.h>

namespace Sequencer {


/** kind of valid line. This can be used to retrieve parameters for
 * the correct kind of line */
enum LineKind {
   LINE_INVALID = 0, /**< syntax error in line */
   LINE_BLANK,       /**< both for blank lines or comment lines */
   LINE_REPEAT,
   LINE_END,
   LINE_PLAYLIST,
   LINE_LIGHT,
   LINE_SEQUENCE,
   LINE_HALL,
   LINE_ENTRY
};

/** parametrs of 'repeat' line */
struct LineParamsRepeat {
   int times;  /**< number of times to repeat a cycle */

   LineParamsRepeat() :
      times(0)
   {}
};

/** sub-commands for command 'playlist' */
enum PlaylistSubCommand
{
   PLAYLIST_NONE = 0,
   PLAYLIST_PLAY,
   PLAYLIST_PLAY_ALL,
   PLAYLIST_PAUSE,
   PLAYLIST_STOP,
   PLAYLIST_SHOW,
   PLAYLIST_HIDE,
   PLAYLIST_SET_TOP,
   PLAYLIST_SET_BOTTOM,
   PLAYLIST_REWIND,
   PLAYLIST_FADEOUT,
   PLAYLIST_VOLUME,
   PLAYLIST_WAIT
};

/** parametrs of 'playlist' line*/
struct LineParamsPlaylist {
   Playlist::Line line;
   PlaylistSubCommand subCommand;
   QString label;
   int numericParam;

   LineParamsPlaylist() :
      line( Playlist::LINE_A),
      subCommand( PLAYLIST_NONE),
      numericParam(0)
   {}
};

enum LightSubCommand
{
   LIGHT_NONE = 0,
   LIGHT_TRIGGER,  /**< trigger a light preset */
   LIGHT_STOP      /**< stop a light animation */
};

/** parametrs of 'light' line*/
struct LineParamsLight {
   LightSubCommand subCommand;
   QString preset;

   LineParamsLight() :
      subCommand( LIGHT_NONE)
   {}
};

enum SequenceSubCommand
{
   SEQUENCE_NONE = 0,
   SEQUENCE_PAUSE,    /**< block sequence for a finite amount of time */
   SEQUENCE_STOP      /**< stop the sequence until user restarts it */
};

/** parametrs of 'light' line*/
struct LineParamsSequence {
   SequenceSubCommand subCommand;
   double timeSec;  /** time to pause in seconds */

   LineParamsSequence() :
      subCommand( SEQUENCE_NONE),
      timeSec( 0.0)
   {}
};

/** parametrs of 'entry' line*/
struct LineParamsEntry {
   QString label;
};

enum HallSubCommand
{
   HALL_NONE = 0,
   HALL_ON,      /**< turn on or dimmer a light */
   HALL_OFF      /**< turn off a light */
};

/** parametrs of 'light' line*/
struct LineParamsHall {
   HallSubCommand subCommand;

   QString zone;  /**< location label */
   int level;  /** range 1-10. Use 0 as default */

   LineParamsHall() :
      subCommand( HALL_NONE),
      level( 0)
   {}
};

/** indicate to repeat a cycle forever */
const int REPEAT_FOREVER = -1;

/** used as delay between instructions to stop sequencer
 * until user re-start it */
const int DELAY_FOREVER = -1;

/** used as delay between instructions  to stop sequence until
 *  an event occours  */
const int DELAY_UNTIL_EVENT = -2;

/** indicate to turn on a hall light at full power */
const int HALL_TRUN_ON_FULL = -1;

/** events that can occour in show and that are notified to
 * Sequencer to trigger sequence evolution */
enum ShowEvents
{
   Media_trackFinished
};

}  // namespace Sequencer

#endif // SEQUENCER_H
