/**
 * @file mouse.h
 * @brief file that has functions to allow configurating and using the mouse
 * 
 */

#ifndef _MOUSE_
#define _MOUSE_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "../keyboard/keyboard.h"

typedef struct {
     int16_t x_pos, y_pos;
     uint8_t click_right_button, click_left_button, click_middle_button;
} MouseHelper;


/**
 * @brief this function subscribes the keyboard interrupts
 * @return 0 if sucess, otherwise 1
 */
int (mouse_subscribe_interrupts)();


/**
 * @brief this function unsubscribes the keyboard interrupts
 * @return 0 if sucess, otherwise 1
 */
int (mouse_unsubscribe_interrupts)();

/**
 * @brief this function writes directly into the mouse the command that is passed
 * @param command information to be written to the mouse
 * @return 0 if sucess, otherwise 1
 */
int (mouse_write_command)(uint8_t command);

/**
 * @brief responsable to detect mouse position and write the position to the byte array of the mouse so parse_packet can write the current mouse positon to the MouseHelper struct
 */
void (assemble_packet)();

/**
 * @brief writes the mouse values into a struct MouseHelper, so its easier to use the mouse as the struct has only the mouse information that is needed in the game
 */
void (parse_packet)();

#endif /* _MOUSE_ */
