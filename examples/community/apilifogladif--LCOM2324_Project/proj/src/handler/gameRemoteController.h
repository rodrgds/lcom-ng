#ifndef _LCOM_GAMEREMOTECONTROLLER_H_
#define _LCOM_GAMEREMOTECONTROLLER_H_

/**
 * @file gameRemoteController.h
 * 
 * @brief Header file for local game drawing functions.
 */


/**
 * @enum THISPLAYER
 * @brief Enumeration of possible remote player states.
 */
enum THISPLAYER {
    PLACING,
    MYTURN,
    WAITING,
    CONNECTING,
    EXIT,
};

/**
 * @brief Function that receives the byte and uses it.
 * 
 * @return int : 0 if sucess and 1 otherwise.
 */
int received(uint8_t val);

/**
 * @brief Checks the keyboard input during the remote game.
 * 
 * @param scancode : The scancode of the key pressed.
 * @return int : Returns 1 if the ESC key was pressed, otherwise returns 0.
 */
int checkKeyboardInputGameRemote(uint8_t scancode);

/**
 * @brief Checks and processes mouse clicks during the remote game.
 */
void checkGameRemoteClick();

#endif
