#ifndef __MOUSE_H
#define __MOUSE_H

/** @defgroup Mouse Mouse
 * @{
 * Functions for manipulating mouse inputs
 */

static int distanceY = 0;

typedef struct
{
	char packet[3];
	unsigned int mouse_left_button;
	unsigned int mouse_middle_button;
	unsigned int mouse_right_button;
	unsigned int overflow_x;
	unsigned int overflow_y;
	unsigned int sign_x;
	unsigned int sign_y;
	int pos_x;
	int pos_y;
} mouse;

mouse mouse_t;

typedef enum
{
	INIT, DRAW, COMPLETE
} state_t;

typedef enum
{
	RDOWN, RUP, MOVE
} ev_type_t;

typedef struct
{
	ev_type_t mode;
} evt;

/**
 * @brief Fills the mouse_t structure with correct values
 */
void setMousePackets();

/**
 * @brief Reads packets from keyboard controller
 */
int mouse_handler();

/**
 * @brief Resets values from the packets
 */
void resetPackets();

/**
 * @brief Checks if the mouse right button is pressed
 * @return 1 on true, 0 otherwise
 */
int isRightButtonPressed();

/**
 * @brief Checks if the mouse left button is pressed
 * @return 1 on true, 0 otherwise
 */
int isLeftButtonPressed();

/**
 * @brief Checks if the movement of the
 * mouse is overflowed or signed
 * @return 1 on success, 0 otherwise
 */
int isCorrectMovement();

/**
 * @brief Subscribes the mouse interruptions
 * @return 1 on error, 0 on success
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribes the mouse interruptions
 * @return 1 on error, 0 on success
 */
int mouse_unsubscribe_int();

/**
 * @brief Writes information through keyboard controller
 * @return 1 on error, 0 success
 */
int writeInfoThroughKBC(unsigned long kbd_port, unsigned long command);

/**
 * @brief Reads information through keyboard controller
 * @return 1 on error, 0 success
 */
int readInfoThroughKBC(unsigned long *data);

/**
 * @brief Mix of writes and reads through keybaord controller
 * @return 1 on error, 0 otherwise
 */
int mouseWrite(unsigned long command);

/**
 * @brief Prints packets on screen
 */
void printPackets();

#endif /**@}*/
