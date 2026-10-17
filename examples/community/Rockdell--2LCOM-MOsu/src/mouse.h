
#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/syslib.h>

/** @defgroup mouse Mouse
 *  @{
 *
 *  Mouse related functions
 */

/**
 *  @brief Subscribes mouse's interruptions.
 *  @return Returns an ID if successful and -1 otherwise.
 */
int mouse_subscribe_int(void );

/**
 *  @brief Unsubscribes mouse's interruptions.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int mouse_unsubscribe_int();

/**
 *  @brief Mouse's interruption handler.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int mouse_int_handler();

/**
 *  @brief Adds a processed byte to a packet.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int addBytePacket(unsigned long byte);

/**
 *  @brief Gets the counter, which keeps track of the number of bytes processed (up to 3 per packet).
 *  @return Returns the counter of packets.
 */
int getCounter();

/**
 *  @brief Check if the right button is pressed (right click).
 *  @return Returns 1 if true and 0 otherwise.
 */
int getRB();

/**
 *  @brief Check if the left button is pressed (left click).
 *  @return Returns 1 if true and 0 otherwise.
 */
int getLB();

/**
 *  @brief Check X-axis' movement.
 *  @return Returns the X-axis' movement.
 */
int getX();

/**
 *  @brief Check Y-axis movement.
 *  @return Returns the Y-axis movement.
 */
int getY();

/**
 *  @brief Prints the last packet processed.
 */
void printPacket();

/**
 *  @brief Updates the static varibles with the last X-axis' movement and Y-axis' movement.
 */
void updateCoord();

/** @} end of mouse */

#endif
