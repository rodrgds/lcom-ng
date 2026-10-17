#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

/**
 * @file mouse.h
 * 
 * @brief This file contains the mouse functions.
*/

/**
 * @brief Mouse interrupt handler.
 * 
 */
void (mouse_ih)();

/**
 * @brief Processes the mouse output.
 * 
 * @return bool : True if a compleated packet is recived and false otherwise
 */
bool (mouse_output)();

/**
 * @brief Subscribes to mouse interrupts.
 * 
 * @param irq_set : IRQ.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (mouse_subscribe_int)(uint8_t *irq_set);

/**
 * @brief Unsubscribes to mouse interrupts.
 * 
 * @return int : 0 if sucess and 1 otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Creates a mouse packet from the byte array
 * 
 */
void (create_mpacket)();

/**
 * @brief Sends a mouse command and waits for an acknowledgment.
 * 
 * @param command : The mouse command to send.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (mouse_data_report)(uint8_t command);

/**
 * @brief Writes a command to the specified port.
 * 
 * @param port : The port to write the command to.
 * @param cmd : The command to write.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (write_command)(uint8_t port, uint8_t cmd);

#endif
