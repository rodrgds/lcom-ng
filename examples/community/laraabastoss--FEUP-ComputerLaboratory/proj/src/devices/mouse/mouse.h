#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "devices/video/graphics.h"
#include "devices/i8042.h"
#include "devices/KBC.h"

typedef struct {
    uint8_t left_click;
    int16_t x;
    int16_t y;
    int16_t delta_x;
    int positive_x;
} MouseInfo;   

/// @brief subscribes the mouse interrupts
/// @return a 0 in case of success and a 1 in case of error with the system call
int (mouse_subscribe_interrupts)();

/// @brief unsuncribes the mouse interrupts
/// @return a 0 in case of success and a 1 in case of error with the system call
int (mouse_unsubscribe_interrupts)();

/// @brief mouse interrupt handler, whenever it receives an interrupt from the mouse it updates the current_byte global variable to the latest received byte
/// @return a 0 in case of success and a 1 in case of error with the system call
void (mouse_interrupt_handler)();

/// @brief if the index is 3, meaning that the 3 bytes of each packet have been received, fills the MouseInfo struct with the information
/// @return a 0 in case of success and a 1 in case of error with the system call
void (mouse_fill_packet)();

/// @brief activates the data reporto to the mouse
/// @return a 0 in case of success and a 1 in case of error with the system call
int (activate_mouse_datareport)();

/// @brief activates the mouse stream
/// @return a 0 in case of success and a 1 in case of error with the system call
int (activate_mouse_stream)();

/// @brief descativates the mouse
/// @return a 0 in case of success and a 1 in case of error with the system call
int (deactivate_mouse)();

#endif
