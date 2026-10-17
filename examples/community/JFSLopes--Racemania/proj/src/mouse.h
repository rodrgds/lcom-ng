#ifndef MOUSE_LAB_4
#define MOUSE_LAB_4

#include <lcom/lcf.h>
#include "mouse_macros.h"

/**
 * @brief Enum representing the various states of a specific process or sequence.
 */
typedef enum {
    INITIAL, /**< Initial state */
    FIRST_LINE, /**< State representing the first line */
    VERTEX, /**< State representing the vertex */
    SECOND_LINE, /**< State representing the second line */
    SUCCESS /**< State representing a successful completion */
} States;

/**
 * @brief Reads and synchronizes the mouse packets
 * 
 * @return OK on success, ERROR otherwise
*/
int (read_packet) ();

/**
 * @brief Subscribes the mouse interrupts
 * 
 * @param bit_no Bit number that will be activated when I interrupt is generated
 * 
 * @return OK on success, ERROR otherwise
*/
int (mouse_unsubscribe_int) ();

/**
 * @brief Unsubscribes the mouse interrupts
 * 
 * @return OK on success, ERROR otherwise
*/
int (mouse_subscribe_int) (uint8_t* bit_no);

/**
 * @brief Organizes the packet information so it's easy to work with them
 * 
 * @param pp Struct that holds the 3 bytes from the mouse packet and also some usefull information related to the mouse state
*/
void (organize_packet) (struct packet* pp);

/**
 * @brief Write a command to the mouse controller
 * 
 * @return OK on success, ERROR otherwise
*/
int (write_command_mouse) (uint8_t command);

/**
 * @brief Checks if a and logical symbol was drawn.
 * 
 * @return OK on success, ERROR otherwise
*/
int (mouse_gesture) (uint8_t x_len, uint8_t tolerance);

/**
 * @brief Reset the packets and state after the packet was processed.
*/
void (reset_packets) ();

#endif
