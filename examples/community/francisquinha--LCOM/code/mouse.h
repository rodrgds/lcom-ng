#ifndef __MOUSE_H
#define __MOUSE_H

/** @file mouse.h
 * @brief file containing declarations of the functions that deal with the game mouse
 */

/** @defgroup mouse mouse
 * @{
 *
 * @brief functions for handling the mouse
 */


/**@{
 * @brief Mouse info
 */
typedef struct {
	int bytes[3];						/**< @brief mouse packet bytes */
	char byte_counter;					/**< @brief mouse packet byte counter */
	char first_packet;					/**< @brief indicator of first mouse packet */
	short x;							/**< @brief mouse horizontal position */
	short y;							/**< @brief mouse vertical position */
	char left;							/**< @brief indicator of pressed left mouse button */
	char middle;						/**< @brief indicator of pressed middle mouse button */
	char right;							/**< @brief indicator of pressed right mouse button */
	char info;							/**< @brief indicator of new mouse info (when the packet 3 bytes are obtained) */
} Mouse;
/** @}*/

/**
 * @brief creates a new mouse
 * @return pointer to the mouse created
 */
Mouse *create_mouse();

/**
 * @brief destroys a previously created mouse
 * @param mouse the mouse to be destroyed
 */
void destroy_mouse(Mouse *mouse);

/**
 * @brief To write a full mouse command and read the mouse acknowledgments
 *
 * Starts with first command to forward argument to mouse and then writes the argument
 * Always reading the mouse replys to check if OK
 *
 * @param cmd argument for the mouse
 *
 * @return 0 if successful, 1 otherwise
 */
int mse_write_read(unsigned long cmd);

/**
 * @brief mouse pointer xpm;
 */
/* XPM */
static char *mouse_xpm[] = {
/* columns rows colors chars-per-pixel */
"11 17 3",
"  0",
". 63",
"X 64",
/* pixels */
".XXXXXXXXXX",
"..XXXXXXXXX",
". .XXXXXXXX",
".  .XXXXXXX",
".   .XXXXXX",
".    .XXXXX",
".     .XXXX",
".      .XXX",
".       .XX",
".        .X",
".     .....",
".  .  .XXXX",
". .X.  .XXX",
"..XX.  .XXX",
".XXXX.  .XX",
"XXXXX.  .XX",
"XXXXXX..XXX"
};

/**
 * @brief to obtain the mouse pointer xpm
 * @return xpm with the mouse pointer
 */
char **get_mouse_xpm();

/**
 * @brief to handle mouse interruptions
 *
 * processes the new packet byte and, if it is the third byte
 * of a packet, computes the mouse information
 *
 * @param mouse whose interruptions we wish to handle
 */
void mouse_int_handler(Mouse *mouse);

/**
 * @brief to make sure the mouse stays inside the screen
 * @param mouse we want to keep inside the screen
 * @param h_res horizontal screen resolution
 * @param v_res vertical screen resolution
 */
void fix_mouse_position(Mouse* mouse, unsigned h_res, unsigned v_res);

/** @} end of mouse */
#endif /* __MOUSE_H */
