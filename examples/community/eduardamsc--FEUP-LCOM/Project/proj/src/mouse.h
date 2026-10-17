#include "obstacleCourse.h"

/**
 * @brief Subscribes and enables mouse
 * @return Bit order in interrupt mask, 1 upon failure to subscribe
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribes mouse
 * @return 0 upon success to unsubscribe, 1 upon failure
 */
int mouse_unsubscribe_int();

/**
 * @brief Sends command to mouse
 * @param cmd Command
 * @return 0 upon success, 1 otherwise
 */
int mouse_write(unsigned long cmd);

/**
 * @brief Reads a byte sent by mouse
 * @param data Byte input from mouse
 * @return 0 upon success, 1 otherwise
 */
int mouse_read(unsigned long* data);

/**
 * @brief Organizes bytes from mouse_read in groups of 3 to put in array packet
 * @param course Game
 * @return 2 upon success, 0 if packet isn't complete
 */
int mouse_reader(ObstacleCourse* course);
