#ifndef MOUSE_H
#define MOUSE_H


#include <stdint.h>

/**
 * @brief Subscribes and enables Mouse interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * 
 * Sets var error to 1 if there was an error
*/
void (mouse_ih)();

/**
 * @brief Checks if there was an error in the interrupt handler.
 * 
 * @return true if there was an error, false otherwise.
*/
bool mouseError();

/**
 * @brief Subscribes and enables Mouse interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * 
 * @return 0 upon success, one otherwise.
*/
int(mouse_subscribe)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Mouse interrupts.
 * 
 * @return 0 upon success, one otherwise.
*/
int(mouse_unsubscribe)(); 

/**
 * @brief Checks if the packet is complete.
 * 
 * @return true if the packet is complete, false otherwise.
*/
int complete_packet();

/**
 * @brief Checks if the packet is complete.
 * 
 * @return true if the packet is complete, false otherwise.
*/
bool isPacketComplete();

/**
 * @brief Gets the packet read from the output buffer.
 * 
 * @return The packet read from the output buffer.
*/
struct packet getMousePacket();

/**
 * @brief Updates the packet with the new byte.
 * 
 * @param mouseByte The new byte to be added to the packet.
 * 
 * @return 0 upon success, one otherwise.
*/
int updatePacket(uint8_t mouseByte);

/**
 * @brief Disables data reporting.
 * 
 * @return 0 upon success, one otherwise.
*/
int disableDataReporting();

#endif
