#ifndef _MOUSE_H
#define _MOUSE_H

#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
#include <stdio.h>
#include "game/include/grid.h"

/**
 * @brief Receives the mouse codes and builds the packet
 * 
 * @param pp pointer to packet
 * @param scode array of codes
 * 
 * @return void function
 */
void (mouse_packet_builder)(struct packet *pp, uint8_t scode[]);

/**
 * @brief Enable/Disable mouse data report
 * 
 * @param set 0 to disable 1 to enable
 * 
 * @return Return a integer between min and max
 */
int (mouse_set_data_report)(int set);

/**
 * @brief Read a byte from mouse
 * 
 * @param byte pointer to byte
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (mouse_read_byte)(uint8_t *byte);

/**
 * @brief Issue a command to mouse
 * 
 * @param cmd Command to be issued
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (mouse_issue_cmd)(uint8_t cmd);

/**
 * @brief verifies if a value is positive or negative
 * 
 * @param byte pointer to byte
 * 
 * @return Returns signed value
 */
int16_t (signed_delta)(uint8_t byte);

#endif /* _MOUSE_H */
