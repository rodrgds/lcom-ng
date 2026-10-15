#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
#include <stdbool.h>

/** @defgroup mouse mouse
 * @{
 * 
 * Functions for the mouse.
 */

/**
 * @brief Subscribes and enables mouse interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes KBD interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Enable mouse data reporting
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_enable)();

/**
 * @brief Disable mouse data reporting
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_disable)();

/**
 * @brief Get status byte from status
 * 
 * @param st address of memory to be initialized with the status byte
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_state)(uint8_t *st);

/**
 * @brief Issuing a command or argument to the KBC
 *
 * @param info command byte or argument
 * @param arg indicates if an argument is being passed
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_cmd)(uint8_t info, bool arg);

/**
 * @brief Read return value/data from the KBC
 *
 * @param data a pointer to the variable that is to be updated with the return data
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_ret_value)(uint8_t *data);

/**
 * @brief Disable data reporting
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_disable_data_reporting)();

/**
 * @brief Checks if the packet bytes' content is full
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_packet_full)();

/**
 * @brief Fill the attributes of the packet struct  
 *
 * @return Return the new packet struct
 */
struct packet (mouse_organize_struct)();

/**
 * @brief Calculate the mouse x displacement   
 *
 * @return Return the value calculated
 */
int16_t (mouse_delta_x)();

/**
 * @brief Calculate the mouse y displacement   
 *
 * @return Return the value calculated
 */
int16_t (mouse_delta_y)();

/**
 * @brief Check if exists mouse overflows    
 *
 * @return Return true upon sucess and false otherwise
 */
int (mouse_overflow)();

/**
 * @brief Get info about a specific mouse interaction    
 *
 * @return Return the struct of the event
 */
struct mouse_ev (mouse_event)();

/**@}*/

#endif /* _LCOM_MOUSE_H */
