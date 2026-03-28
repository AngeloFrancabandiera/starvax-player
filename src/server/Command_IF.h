#ifndef COMMAND_IF_H
#define COMMAND_IF_H

#include "qglobal.h"

namespace Server {

/**
 * @{ list of supported commands
 */
const quint8 CONNECT = 1u;
const quint8 SET_CREDENTIALS = 2u;
const quint8 DISCONNECT = 3u;

const quint8 GET_SHOW_TITLE = 10u;
const quint8 GET_MEDIA_LIST = 11u;
const quint8 GET_LIGHT_LIST = 12u;
const quint8 GET_SEQUENCE_ENTRIES_LIST = 13u;
const quint8 GET_OPEN_WEB_NET_LIST = 14u;

const quint8 SET_ACTIVE_MEDIA = 20u;
const quint8 PLAY_MEDIA = 21u;
const quint8 PAUSE_MEDIA = 22u;
const quint8 STOP_MEDIA = 23u;
const quint8 REWIND_MEDIA = 24u;


const quint8 CONTROL_LIGHT_PRESET = 30u;

const quint8 SET_ACTIVE_SEQUENCE_ENTRY = 40u;
const quint8 CONTROL_SEQUENCE = 41u;

const quint8 CONTROL_OPEN_WEB_NET = 50u;
/** @} */


/**
 * @brief The Command_IF class is the interface for all commands that can
 *   be sent from Network interface.
 *
 * Every command has a 16 bit operative code and a number of null terminated
 * strings as parameters.
 */
class Command_IF
{
public:
   virtual ~Command_IF() {}

   /**
    * @return the operative code managed by the concrete subclass.
    * Two different subclasses should have different operative code; otherwise
    * the behaviour is not defined: only one command may be managed by the engine
    * or all subclasses may be executed.
    */
   virtual quint8 operativeCode() const = 0;

   /**
    * @brief execute a command accodring to subclass operative code and \p parameters
    * @param parameters is a set of strings that specify how to execute the command.
    * @return true if the command has been executed succesfully.
    */
   virtual bool execute( const QStringList & parameters) = 0;

   /**
    * @brief this function can optionally be redefined to allow some commands
    *   to be executed without providing password.
    * @return true if command can be executed without granting password
    */
   virtual bool isAllowedWithoutGrant() const {
      return false;
   }
};

}  // namespace Server

#endif // COMMAND_IF_H

