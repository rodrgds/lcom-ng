/**
	@file mouse.h
	@author Daniel Marques
	@author João Almeida

	Prototypes of the functions used to communicate with mouse
*/

#ifndef MOUSE_H
#define MOUSE_H

/** @defgroup mouse mouse
 * @{
 *
 * Functions responsible for communicating with the mouse
 */

#include "game.h"

//TOOLS
#define BIT(n) (0x1<<(n))

//HARDWARE
#define MOUSE_IRQ 12
#define STAT_REG 0x64
#define OUT_BUF 0x60
#define IN_BUF 0x60

//CONVENTIONS
#define IBF BIT(1)
#define OBF BIT(0)
#define EN_MOUSE 0xA8
#define MOUSE_CMD 0xD4
#define STREAM_MODE 0xF4
#define DATA_OFF 0xF5

#define Y_SIGN BIT(5)
#define X_SIGN BIT(4)


//FUNCTIONS

/**
  @brief Subscribes mouse interrupts
  @param mouse_id To be ID of the mouse interrupts
  @details Imported from lab4
 */
void mouseSubscribe(int mouse_id);

/**
  @brief Unsubscribes mouse interrupts
  @param mouse_id ID of the mouse interrupts
  @details Imported from lab4
 */
void mouseUnsubscribe(int mouse_id);

/**
  @brief Sends a command to a given port
  @param port Where to send the command to
  @param cmd Command to send
  @return 0 at success -1 at failure
  @details Imported from lab4
 */
int sendCmd(unsigned long int port, unsigned long int cmd);

/**
  @brief Receives a byte from the KBC
  @param data To be byte read from the KBC
  @return 0 at success -1 at failure
  @details Imported from lab4
 */
unsigned long int receiveData(unsigned long int * data);

/**
  @brief Receives the byte sent by the KBC that represents a mouse action
  @param packet To be packet read from the KBC
  @param byte_counter How many bytes have been received for a single mouse action
  @return 1 if the bytes are not yet synched , 0 otherwise
  @details Imported from lab4
 */
int readMouseByte(unsigned char *packet, unsigned short int byte_counter);

/**
  @brief Updates the mouse coordinates based on the values in the packets
  @param game Struct that holds all information about the game
  @param packet Array with information of the mouse interrupt
*/
void updateMouseCoordinates(game_t *game , unsigned char packet[]);

/**
  @brief Checks if mouse is above any main menu button
  @param game Struct that holds all information about the game
  @param relevant If the game is in main menu or not (1 = MAIN MENU , 0 = ANY OTHER STATE)
  @return Button mouse is hovering (0 -> PLAY , 1 -> EXIT , 2 -> NONE/IRRELEVANT)
*/
unsigned int mouseAboveButton(game_t *game , unsigned int relevant);
#endif
