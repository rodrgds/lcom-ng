#ifndef MOUSE_H
#define MOUSE_H
#include <lcom/lcf.h>


/** @defgroup mouse 
 * @{
 *
 * Functions related to mouse 
 */

/**
 * @brief Waits the mouse delay
 * 
 */
void mouse_delay();

/**
 * @brief Disable mouse data report
 * 
 * @return int 
 */
int disableMouseDataReporting();

/**
 * @brief Enable mouse data reporting
 * 
 * @return int 
 */
int enableMouseDataReporting();

/**
 * @brief Wryte byte to mouse
 * 
 * @param command 
 * @return int 
 */
int writeByteToMouse(uint32_t command); 

/**
 * @brief Enable mouse streaming
 * 
 * @return int 
 */
int enableMouseStreaming();

/**
 * @brief Disable mouse streaming
 * 
 */
void disableMouseStreaming();

/**
 * @brief Subscribe mouse interrupts
 * 
 * @param hook_id 
 * @return int 
 */
int mouse_subscribe_int(uint8_t *hook_id);

/**
 * @brief Unsubscribe mouse interrupts
 * 
 * @return int 
 */
int mouse_unsubscribe_int();

/**
 * @brief Check if input buffer is full
 * 
 * @return int 
 */
int check_input_buffer();

/**
 * @brief Check if outputbuffer is available
 * 
 * @return int 
 */
int check_output_buffer();

/**
 * @brief Read a packet from mouse
 * 
 */
void mouse_read();

/**
 * @brief Mouse interrupt handler
 * 
 */
void (mouse_ih)();

/**
 * @brief Returns true if 3  packets were read
 * 
 * @return true 
 * @return false 
 */
bool isMouseDataPacketReady();

/**
 * @brief Return the data packet that was read
 * 
 * @return struct packet 
 */
struct packet readMouseDataPacket();



#endif
