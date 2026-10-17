/**
 * @file mouse.h
 * @brief Header file for the mouse controller module.
 */

#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

/**
 * @brief Struct to hold the state of mouse buttons.
 */
typedef struct{
  uint8_t left;
  uint8_t last_left;
  uint8_t right;
  uint8_t last_right;
  uint8_t middle;
  uint8_t last_middle;
} click_info;

/**
 * @brief Subscribes and enables mouse interrupts.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (mouse_subscribe)();

/**
 * @brief Unsubscribes mouse interrupts.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (mouse_unsubscribe)();

/**
 * @brief Mouse interrupt handler.
 * 
 * Reads the mouse byte from the KBC output port and stores it in a global variable.
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (mouse_ihandler)();

/**
 * @brief Configures the mouse.
 * 
 * @param control_word Control word to configure the mouse.
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (mouse_config)(uint8_t control_word);

/**
 * @brief Synchronizes the mouse bytes.
 * 
 * Stores the mouse byte in a global array and increments the byte counter.
 */
void (mouse_sync_bytes)();

/**
 * @brief Converts mouse bytes to a package.
 * 
 * Converts the global mouse bytes array to a package and updates the mouse button states.
 * @return Returns 0 upon success.
 */
int (mouse_bytes_to_package)();

/**
 * @brief Updates the mouse location.
 * 
 * Updates the global mouse x and y coordinates based on the mouse package.
 */
void (update_mouse_location)();

/**
 * @brief Checks if the left mouse button was released.
 * 
 * @return Returns 1 if the left mouse button was released and 0 otherwise.
 */
bool (left_button_released)();

#endif
