/**
@file keyboard.h
@brief Functions and macros related to keyboard communication. */

#ifndef KEYBOARD_H 
#define KEYBOARD_H

#include <lcom/lcf.h> 
#include "i8042.h"

/**
@brief Keyboard interrupt handler. */ 
void (kbc_ih)();

/**
@brief Subscribes keyboard interrupts.
@param irq_set Pointer to the subscription bitmask.
@return int Returns 0 upon success and non-zero otherwise. */
int (kbc_subscribe)(uint8_t *irq_set);

/**
@brief Unsubscribes keyboard interrupts.
@return int Returns 0 upon success and non-zero otherwise. */
int (kbc_unsubscribe)();

/**
@brief Reads data from the KBC buffer.
@param port KBC port to read from.
@param data Pointer to the variable that will hold the read data.
@return int Returns 0 upon success and non-zero otherwise. */
int kbc_read_buffer(uint8_t port, uint8_t *data);

/**
@brief Reads data from the KBC buffer assuming it contains a scancode.
@param scancode Pointer to the variable that will hold the read scancode.
@return int Returns 0 upon success and non-zero otherwise. */
int kbc_read_data(uint8_t *scancode);

/**
@brief Reads the KBC status byte.
@param st Pointer to the variable that will hold the status byte.
@return int Returns 0 upon success and non-zero otherwise. */
int (kbc_get_status)(uint8_t *st);

/**
@brief Restores the interrupts after disabling them during command handling.
@return int Returns 0 upon success and non-zero otherwise. */
int (kbc_restore_interrupts)();

/**
@brief Writes a command to a KBC port.
@param port KBC port to write to.
@param cmd Command to be sent.
@return int Returns 0 upon success and non-zero otherwise. */
int (kbc_write_cmd)(int port, uint8_t cmd);

#endif //KEYBOARD_H
