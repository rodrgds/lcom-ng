#pragma once

#include "i8254.h"
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "timer.h"
#include "utils.h"
#include "i8254.h"
#include "videocard.h"
/**
* @brief Mouse's interrupt handler
*
* Handles mouse interrupts
*
* @return Returns nothing
*/
void (mouse_ih)();
/**
* @brief Updates mouse position
*
* Updates nouse position and restricts mouse to the proper screen bounds
* @param pp - pointer to packet of struck packet type
* @return Returns nothing
*/
void updateMousePosition(struct packet *pp);
/**
* @brief Mouse subscription
*
* Subscribes mouse interrupts
* @param bit_no - pointer to the bit_no to be set for interrupt detection
* @return Returns 0 on success, 1 otherwise
*/
int mouse_subscribe_int(uint8_t* bit_no);
/**
* @brief Mouse unsubscription
*
* Unsubscribes mouse interrupts
* @return Returns 0 on success, 1 otherwise
*/
int mouse_unsubscribe_int();
/**
* @brief Parses mouse packet 
*
* Parses mouse packet given and extracts information properly
* @param packet - pointer to packet of struck packet type
* @return Returns 0 on a success, 1 othwerwise 
*/
int parse_packet(struct packet* packet);
/**
* @brief 9 bit two's complement converion
*
* Auxiliary function to make mouse work properly - 8 bit to 9 bit two's complement conversion
* 
* @param value - value to be converted
* @param msb - value's most significant bit (0 or 1)
* @return Returns a 16 bit integer representing converted value
*/
int16_t conversion(uint8_t value, uint8_t msb);
/**
* @brief Mouse polling
* @return Returns nothing
*/
void mouse_polling();
/**
* @brief Writes command
*
* Writes given command to appropriate register to change mouse settings
*
* @param command - command to write
* @return Returns 0 on a success, 1 otherwise
*/
int write_command(uint8_t command);
