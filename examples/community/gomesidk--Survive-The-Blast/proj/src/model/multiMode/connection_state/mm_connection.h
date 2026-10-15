#ifndef MM_CONNECTION_H
#define MM_CONNECTION_H

#include <lcom/lcf.h>
#include "spriteLoader.h"
#include "cursor.h"
#include "serial_port.h"
#include "sprite.h"
#include "button.h"

/**
 * @file mm_connection.h
 * @brief Multiplayer mode connection state management
 * 
 * This module provides functions to create, manage, and draw the connection state in the multiplayer mode.
 * It handles the connection process, including sending and receiving handshake messages, and processing input from the mouse and timer. 
 */

struct mm_connection_imp;
typedef struct mm_connection_imp MmConnection;

#define HANDSHAKE_HELLO  0xC1
#define HANDSHAKE_REPLY  0xC2

/**
 * @brief Creates a new MmConnection object
 * 
 * This function initializes the connection state, including the background, buttons, and sprites.
 * It also clears the serial port buffer and sets the initial time until failure.
 * 
 * @param loader Pointer to the SpriteLoader object
 * @return Pointer to the created MmConnection object, or NULL if the creation failed
 */
MmConnection *create_mm_connection(SpriteLoader *loader);

/**
 * @brief Destroys a MmConnection object, freeing all allocated memory
 * 
 * This function cleans up the resources used by the MmConnection object, including buttons and sprites.
 * 
 * @param mm_connection Pointer to the MmConnection object to be destroyed
 */
void destroy_mm_connection(MmConnection *mm_connection);

/**
 * @brief Draws the MmConnection screen
 * 
 * This function draws the background, buttons, and sprites for the connection state.
 * It displays the waiting connection and warning connection sprites.
 * 
 * @param mm_connection Pointer to the MmConnection object to be drawn
 */
void draw_mm_connection(MmConnection *mm_connection);

/**
 * @brief Processes the input from the mouse in the MmConnection state
 * 
 * @param mm_connection Pointer to the MmConnection object
 * @param c Pointer to the Cursor object
 * @return 0 if the connection state should continue, 1 to go back to the menu
 */
int process_mm_connection_mouse(MmConnection *mm_connection, Cursor *c);

/**
 * @brief Processes the input from the timer in the MmConnection state
 * 
 * @param mm_connection Pointer to the MmConnection object
 */
int process_mm_connection_timer(MmConnection *mm_connection);

/**
 * @brief Processes the input from the serial port in the MmConnection state  
 * 
 * @param mm_connection Pointer to the MmConnection object
 * @param byte The byte received from the serial port
 * @return 0 if the connection state should continue, 1 to go back to the menu, 2 for player 2
 */
int process_mm_connection_sp(MmConnection *mm_connection, uint8_t byte);

#endif // MM_CONNECTION_H
