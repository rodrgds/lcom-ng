/**
 * @file mouse.h
 * @author LCOM professors
 * @brief Mouse device handling
 * @version 0.1
 * @date 2021-01-05
 * 
 * Adapted by Bernardo Santos (bernardo.ots@fe.up.pt)
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>

/** @defgroup mouse Mouse
 * @{
 *
 * Mouse device handling
 */

/**
 * @brief Handle mouse interrupts.
 * 
 */
void(mouse_ih)();

/**
 * @brief Subscribe to mouse interrupts.
 * 
 * @param bit_no 
 * @return 0 on success, 1 on failure 
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe from mouse interrupts.
 * 
 * @return 0 on success, 1 on failure 
 */
int(mouse_unsubscribe_int)();

/**
 * @brief Parse the mouse raw bytes and set the mouse packet attributes.
 * 
 * @param pp mouse packet with attributes to be set
 * @param mouseBytes mouse raw bytes
 */
void parsePacket(struct packet *pp, uint8_t mouseBytes[]);

/**
 * @brief Set the mouse data report mode (enable, disable, set stream mode, etc.).
 * 
 * @param entry value to set on the register
 * @return 0 on success, 1 on failure 
 */
int mouse_data_report(uint8_t entry);

/** @} end of mouse */

#endif
