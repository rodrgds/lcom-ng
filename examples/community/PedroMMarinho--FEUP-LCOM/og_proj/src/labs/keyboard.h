
#pragma once

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Subscribes and enables Keyboard interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * 
 * @return 0 upon success, one otherwise.
*/
void(kbc_ih)();

/**
 * @brief Checks if there was an error in the interrupt handler.
 * 
 * @return true if there was an error, false otherwise.
*/
bool keyboardIHError();

/**
 * @brief Reads the scancode from the output buffer.
 * 
 * @return The scancode read from the output buffer.
*/
int(keyboard_subscribe)(uint8_t *bit_no); 

/**
 * @brief Unsubscribes Keyboard interrupts.
 * 
 * @return 0 upon success, one otherwise.
*/
int(keyboard_unsubscribe)(); 

/**
 * @brief Gets the scancode read from the output buffer.
 * 
 * @return The scancode read from the output buffer.
*/
uint8_t getKeyboardScancode(); 
