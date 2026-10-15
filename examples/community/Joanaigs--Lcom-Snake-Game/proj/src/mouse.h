#ifndef __MOUSE_H
#define __MOUSE_H
#include <minix/sysutil.h>
#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
int hook_idR;
int mouse_number_bytes;
uint8_t mouse[3];
uint8_t statusCode;
uint8_t scanC;

/**
 * @brief Subscribes and enables mouse interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @param hook hookId value
 * @return int Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe(uint8_t *bit_no, int hook);

/**
 * @brief 
 * 
 * @return struct packet 
 */
struct packet parse_packet();

/**
 * @brief Enables/disables stream mode data reporting, by sending the respective command to the mouse.
 * 
 * @param if true enable, if false disable
 * @return int O on success, and non-zero otherwise
 */
int mouse_enable_data_report(bool enable);

/**
 * @brief Handles mouse interrutps.
 * 
 */
void mouse_ih();

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe();

/**
 * @brief creates a mouse packet 
 * 
 * @param pp address of a struct packet with a mouse packet

 * @return struct mouse_ev address of the mouse event detected
 */
struct mouse_ev mouse_get_event(struct packet *pp) ;

#endif /* __MOUSE_H */
