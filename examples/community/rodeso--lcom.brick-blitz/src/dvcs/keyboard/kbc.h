/**
 * @file kbc.h
 * @brief Header file for keyboard controller (KBC) functions and constants.
 *
 * This file contains the declarations of functions and constants related to the keyboard controller (KBC).
 * It provides functionality to handle keyboard interrupts, read scancodes, and send commands to the keyboard.
 */

#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_
#pragma once

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#define IRQ0_VECTOR     0x50
#define IRQ1_VECTOR     0x01

#define KEYBOARD_IRQ_LINE    1
#define READ_KEYBOARD_STATUS 0x64
#define INPUT_BUFFER_KEYBOARD 0x64
#define OUTPUT_BUFFER_KEYBOARD 0x60
#define ESC_SCANCODE       0x81
#define BREAK_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define ENABLE_INT      BIT(0)
#define TIMEOUT_ERROR   BIT(6)
#define PARITY_ERROR    BIT(7)
#define OUTPUT_BUFFER_FULL BIT(0)
#define INPUT_BUFFER_FULL  BIT(1)
#define READ_COMMAND   0x20
#define WRITE_COMMAND   0x60
#define DELAY        20000

/**
 * @brief Subscribes and enables keyboard interrupts.
 *
 * @param bit_no Pointer to the variable that will store the keyboard interrupt bit mask.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Enables interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (enable_interrupts)();

/**
 * @brief Reads the scancode from the specified port.
 *
 * @param port The port to read the scancode from.
 * @param result Pointer to the variable that will store the read scancode.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (read_scancode)(uint8_t port, uint8_t* result);

/**
 * @brief Writes a command to the specified port.
 *
 * @param port The port to write the command to.
 * @param the_command The command to be written.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (write_command)(uint8_t port, uint8_t the_command);

#endif
