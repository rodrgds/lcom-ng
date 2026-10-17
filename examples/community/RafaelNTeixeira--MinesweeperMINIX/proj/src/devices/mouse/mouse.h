#pragma once
#include <lcom/lcf.h>
#include "statemachine.h"

#define DELAY_US2 25000

extern struct packet pkt;

/**
 * @brief This function checks the mouse input buffer
 * @return Return true if case of success, false otherwise
*/
bool (check_input_buffer)();

/**
 * @brief This function checks the mouse output buffer 
 * @return Return true if case of success, false otherwise
*/
bool (check_output_buffer)();

/**
 * @brief This function uses irq_setpolicy to configure mouse interrupts
 * @param [out] bit_no All zeros except for the bit position of the timer's hook id
 * @return Return 0 if case of success, 1 if otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief This function unsubscribes mouse interrupts
 * @return Return 0 if case of success, 1 if otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse's input handler
 */
void (mouse_ih)();

/**
 * @brief When mouse packet array has stored all the necessary bytes this functions interprets it and updates mouse information
 * @param pacote An object of the type packet.
 */
void (buildPacket)(struct packet * pacote);

/**
 * @brief This function enables data reporting from the mouse by sending the appropriate command to the mouse
 * @return Return 0 if case of success, 1 if otherwise
*/
int (mouse_enable_data_report)();

/**
 * @brief This function disables data reporting from the mouse device by sending the appropriate command to the mouse 
 * @return Return 0 if case of success, 1 if otherwise
*/
int (mouse_disable_data_report)();

/**
 * @brief This function writes a command to the mouse and retrieves the response from the mouse device
 * @param cmd the command to be written to the mouse
 * @param resp pointer to a variable where the response from the mouse will be stored
 * @return Return 0 if case of success, 1 if otherwise
*/
int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp);

/**
 * @brief This function enables the mouse to send data packets in stream mode
 * @return Return 0 if case of success, 1 if otherwise
*/
int (mouse_stream)();

/**
 * @brief This function reads data from the mouse device
 * @return Return 0 if case of success, 1 if otherwise
*/
int (mouse_read_data)();
