#ifndef _SP_PROTOCOL_H_
#define _SP_PROTOCOL_H_

/**
 * @ingroup Game
 * @{
 * 
 * @defgroup protocol Serial Port Protocol
 * 
 * @brief Codes used in the serial port protocol
 * 
 * For communication from the drawing state to the guessing state, each of these codes must be sent twice everytime they are used, 
 * so that they are not mistaken for mouse data.
 * 
 * @{
*/

#define READY_DRAW      0xFF    /**< @brief The player chose the DRAW option on the multiplayer menu*/
#define READY_GUESS     0XFE    /**< @brief The player chose the GUESS option ont he multiplayer menu*/
#define UNREADY         0XFD    /**< @brief The player went back to the previous state*/

#define PENCIL_SIZE     0xFC    /**< @brief While drawing, the player chose one of the options to modify the size of the pencil*/
#define SCREEN_CLEAR    0xFB    /**< @brief While drawing, the player chose the option to clear the screen*/
#define PENCIL_COLOR    0xFA    /**< @brief While drawing, the player chose one of the options to modify the colour of the pencil*/

#define RIGHT_GUESS     0XF9    /**< @brief The submitted guess is correct*/
#define WRONG_GUESS     0XF8    /**< @brief The submitted guess is wrong*/

#define SUBMIT_MESSAGE  0xF7    /**< @brief Submit the word that was written */
#define DEL_MESSAGE     0xF6    /**< @brief Delete a character from the word that was written */

/**
 *  }@
 * }@
*/

#endif
