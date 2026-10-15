#ifndef MOUSE_H
#define MOUSE_H

#include "../macros/includes.h"


/**
 * @brief Subscribes the mouse device, setting into the interrupt mask the respective mouse hook id bit to 1
 * @return Returns the bit number that was set if successfull or 0xFF if unsuccessfull
 */
uint8_t mouse_subscribe();
/**
 * @brief Unsubscribes the mouse device, setting into the interrupt mask the respective mouse hook id bit to 0
 * @return Returns 0 if successfull and 1 if otherwise
 */
int mouse_unsubscribe();
/**
 * @brief Writes to the mouse a certain command to allow us to use the mouse in a way the user wants
 * @param command Command to write to mouse
 * @return Returns 0 if successfull and 1 if otherwise
 */
int mouse_write(uint8_t command);

/**
 * @brief Reads the value of input made by user from a port given by argument
 * @param port Port from which value is being  read
 * @return Returns the value of input made if successfull and 0xFFFF if otherwise
 */
int mouse_read_output(int port);
/**
 * @brief Writes the value of a command to a port given by argument
 * @param port Port from which value is being written
 * @param command Command to write to port
 * @return Returns 0 if successfull and 0xFFFF if otherwise
 */
int mouse_write_output(int port, unsigned long command);

/**
 * @brief Handles the interrupts of the mouse
 * @return Returns 0 if successfull and 0xFF if otherwise
 */
int mouse_control();
/**
 * @brief Fills the struct mouse_packet with values of the inputs of the mouse
 * @param scancode The value of the inputs the user gives
 */
void fill_mouse_packet(uint32_t scancode);

/**
 * @brief Moves the mouse accordingly based on the values stores in the mouse_packet
 */
void move_mouse();
/**
 * @brief Checks if mouse is colliding with coordinates given by argument
 * @param xi left horizontal coordinate of collision target
 * @param yi up vertical coordinate of collision target
 * @param xf right horizontal coordinate of collision target
 * @param yf down vertical coordinate of collision target
 * @return Returns true if there is collision and false if otherwise
 */
bool mouse_position(unsigned int xi, unsigned int yi, unsigned int xf, unsigned int yf);

#endif
