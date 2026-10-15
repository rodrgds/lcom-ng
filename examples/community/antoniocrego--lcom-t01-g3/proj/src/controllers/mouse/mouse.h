#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include "../utils/kbc.h"
#include <lcom/lcf.h>

/// @brief Mouse interrupt handler
void (mouse_ih)();

/// @brief Stores the mouse interrupt correctly
void (mouse_sync_bytes)();

/// @brief Writes a command to mouse
/// @param command Command to write
/// @return Returns 0 on success and 1 otherwise 
int (write_to_mouse)(uint8_t command);

/// @brief Subscribes the interrupts for mouse
/// @param bit_no The bit of mouse's hook id
/// @return Returns 0 on success and 1 otherwise
int (mouse_subscribe_interrupts)(uint8_t* bit_no);

/// @brief Unsubscribes the interrupts for mouse
/// @return Returns 0 on success and 1 otherwise
int (mouse_unsubscribe_interrupts)();

/// @brief Updates the packet info once the mouse has stored all the information needed
void (make_packet)();

#endif
