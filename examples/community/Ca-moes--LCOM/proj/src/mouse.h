#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the i8042 PS/2 Mouse
 */

/**
 * @brief Subscribe and enable Mouse's interrupts
 * 
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */

int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe Mouses's interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Construct a packet struct with the bits of a 3 byte packet
 * 
 * @param p address of memory to be initialized with the
 *         packet struct
 */
void build_packet_struct(struct packet *p, uint8_t *packet);

/**
 * @brief Enable Data Reporting
 * 
  * @return Return 0 upon success and non-zero otherwise
 */
int mouse_enable_data_rep();

/**
 * @brief Enable Data Reporting
 * 
  * @return Return 0 upon success and non-zero otherwise
 */
int mouse_disable_data_rep();

/**
 * @brief Write a Mouse Command(reg 0xD4) - written as argument of Kbc Write Mouse byte command
 * 
  * @return Return 0 upon success and non-zero otherwise
 */
int mouse_write_command(uint8_t cmd);

/**
 * @brief Check the acknowledgment byte sent by the mouse controller
 *  
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_check_ack(uint8_t *ackB);

/**@brief Mouse event enumeration for the actual state*/
typedef enum{
    INIT,       /**<@brief initial state of the movement*/
    DRAWDOWN,   /**<@brief drawing line down*/
    VERTEX,     /**<@brief vertex of the shape*/
    DRAWUP,     /**<@brief drawing line up*/
}state_t ;

/**@brief Mouse event enumeration type, relevant for gesture detection*/
typedef enum{
    LEFT_DOWN,  /**<@brief pressed left button*/
    LEFT_UP,    /**<@brief left button not pressed*/
    RIGHT_DOWN, /**<@brief pressed right button*/
    RIGHT_UP,   /**<@brief right button not pressed*/
    INVALID     /**<@brief movement that invalidates the gesture we are trying to detect*/
}ev_type_t;

#endif /* __MOUSE_H */
