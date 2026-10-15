/**
 * @file keyboard.c
 * @brief Keyboard driver implementation for handling keyboard interrupts and communication with the i8042 controller.
 *
 * This file provides functions for subscribing/unsubscribing keyboard interrupts,
 * handling keyboard interrupts, and sending/receiving commands and arguments to/from the keyboard controller.
 */

#include <lcom/lcf.h>
#include <lcom/utils.h>
#include "keyboard.h"
#include "Drivers/Utils/utils.h"
#include "i8042.h"

static int hook_id = 1;

uint8_t scancode[2];       
uint8_t scancode_size = 1; 
bool error = false;        
bool scancode_ready = false;

/**
 * @brief Subscribes keyboard interrupts.
 *
 * Sets the IRQ policy for keyboard interrupts and enables exclusive access.
 *
 * @param bit_no Pointer to variable where the hook id bit number will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (kbd_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id;
    return sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

/**
 * @brief Unsubscribes keyboard interrupts.
 *
 * Removes the IRQ policy for keyboard interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (kbd_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_id);
}

/**
 * @brief Keyboard interrupt handler.
 *
 * Reads the status and output buffer from the keyboard controller.
 * Handles single and two-byte scancodes, and sets error flags if necessary.
 */
void (kbc_ih)() {
    uint8_t status, data;

    util_sys_inb(STAT_REG, &status);

    if ((status & (PAR_ERR | TO_ERR)) != 0) {
        error = true;
        return;
    }

    if (status & OBF) {
        if (util_sys_inb(OUT_BUF, &data) != OK) return;

        error = false;

        if (data == 0xE0) {
            scancode[0] = data;
            scancode_size = 2;
            scancode_ready = false; // wait for second byte
        } else {
            if (scancode_size == 2) {
                scancode[1] = data;
                scancode_ready = true; // now the full 2-byte scancode is ready
            } else {
                scancode[0] = data;
                scancode_size = 1;
                scancode_ready = true; // single-byte scancode ready
            }
        }
    }
}

/**
 * @brief Writes a command to the keyboard controller.
 *
 * Tries up to 5 times to write the command if the input buffer is not full.
 *
 * @param cmd Command byte to write.
 * @return 0 on success, 1 on failure.
 */
int (kbc_write_command)(uint8_t cmd) {
    uint8_t status;
    for (int i = 0; i < 5; i++) {
        if (util_sys_inb(KBC_STAT_REG, &status) != OK)
            continue;
        if ((status & KBC_IBF) == 0) {
            return sys_outb(KBC_CMD_REG, cmd);
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}

/**
 * @brief Writes an argument to the keyboard controller.
 *
 * Tries up to 5 times to write the argument if the input buffer is not full.
 *
 * @param arg Argument byte to write.
 * @return 0 on success, 1 on failure.
 */
int (kbc_write_argument)(uint8_t arg) {
    uint8_t status;
    for (int i = 0; i < 5; i++) {
        if (util_sys_inb(KBC_STAT_REG, &status) != OK)
            continue;
        if ((status & KBC_IBF) == 0) {
            return sys_outb(KBC_ARG_REG, arg);
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}

/**
 * @brief Reads output from the keyboard controller.
 *
 * Tries up to 5 times to read the output if the output buffer is full.
 *
 * @param output Pointer to variable where the output byte will be stored.
 * @return 0 on success, 1 on failure.
 */
int (kbc_read_output)(uint8_t *output) {
    uint8_t status;
    for (int i = 0; i < 5; i++) {
        if (util_sys_inb(KBC_STAT_REG, &status) != OK)
            continue;
        if (status & KBC_OBF) {
            return util_sys_inb(KBC_OUT_BUF, output);
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}

