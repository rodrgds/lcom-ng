#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "../../vector2.h"

#define MOUSE_IRQ 12

#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)

#define MOUSE_SIGN_X BIT(4)
#define MOUSE_SIGN_Y BIT(5)

#define MOUSE_OVFX BIT(6)
#define MOUSE_OVFY BIT(7)

#define MOUSE_CMD 0xD4
#define MOUSE_ACK 0xFA


/**
 * Subscribe to mouse interrupts
 * @param bit_no placeholder for hook id
 * @return 0 if successful, non-0 otherwise
*/
int mouse_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribe to mouse interrupts
 * @return 0 if successful, non-0 otherwise
*/
int mouse_unsubscribe_int();

//void set_mouse_sample_rate();

/**
 * @brief Parse a mouse data packet
 * @param pp packet pointer
 * @return 0 if successful, 1 if incomplete, 2 if invalid (error)
*/
int mouse_parse_packet(struct packet *pp);

/**
 * @brief Execute a mouse command
 * @param cmd command to be written
 * @return 0 if successful, non-0 otherwise
*/
int mouse_write_cmd(uint8_t cmd);

/**
 * @brief Enable data reporting for the mouse
 * @return 0 if successful, non-0 otherwise 
 * @warning Disable interrupts, or else command
 * response will wrongly generate an interrupt
*/
int mouse_enable_data_rep();

/**
 * @brief Disable data reporting for the mouse
 * @return 0 if successful, non-0 otherwise 
 * 
*/
int mouse_disable_data_rep();

/**
 * @brief Enable mouse stream mode
 * @return 0 if successful, non-0 otherwise 
*/
int mouse_enable_stream();
