#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>


/**
 * @brief Reads and stores the mouse packet
*/
void (mouse_ih)();

/**
 * @brief Subscribes and enables Mouse interrupts
 * 
 * @param bit variable that will store the mouse hook id
 * @return int 0 if success, 1 otherwise
*/
int (mouse_subscribe_int)(uint8_t *bit);

/**
 * @brief Unsubscribes Mouse interrupts
 * 
 * @return int 0 if success, 1 otherwise
*/
int (mouse_unsubscribe_int)();

// void (mouse_sync_packet)();

/**
 * @brief Saves the mouse packet in a struct
 * 
 * @param mouse_packet struct that will store the mouse packet
*/
void (mouse_bytes_to_packet)(struct packet *mouse_packet);

/**
 * @brief Writes a command to the KBC
 * 
 * @param cmd command to be written
 * @return int 0 if success, 1 otherwise
*/
int (mouse_write)(uint8_t cmd);

/**
 * @brief Tries to read the KBC output buffer and stores it in the variable output
 *  
 * @param bit variable that will store the mouse hook id
 * @return int 0 if success, 1 otherwise
 */
int (KBC_read_output_mouse)(uint8_t port, uint8_t* output);

/**
* @brief Tries to write a command to the KBC
*
* @param port port to be written
* @param cmdByte command to be written
* @return int 0 if success, 1 otherwise
*/
int (KBC_write_cmd)(uint8_t port, uint8_t cmdByte);

#endif


