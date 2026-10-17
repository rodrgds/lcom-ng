/**
 * @file KBC.h
 *
 */

#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

#define DELAY_US 20000

/* KBC COMMANDS */
// written to the command register
#define KBC_GET_COMMAND 0x20
#define KBC_SET_COMMAND 0x60
#define KBC_FORWARD_TO_MOUSE 0xD4

// written to the input buffer
#define KBC_ENABLE_DATA_REPORT 0xF4
#define KBC_DISABLE_DATA_REPORT 0xF5

/**
 * @brief struct representing the status of the KBC
 * @param parity_error indicates if there was a parity error
 * @param timeout_error indicates if there was a timeout error
 * @param mouse_data indicates if the value read corresponds to the mouse or not
 * @param ibf_full indicates if the input buffer of the KBC is full
 * @param obf_full indicates if the output buffer of the KBC is full
 */
typedef struct {
    bool parity_error;
    bool timeout_error;
    bool mouse_data;
    bool ibf_full;
    bool obf_full;
} kbc_status;

static uint8_t st; // status

// status functions
/**
 * @brief reads the KBC status register
 * @return zero on success, non-zero otherwise
 */
int (kbc_get_status)();

/**
 * @brief parses the information regarding the status of the KBC
 * @return struct representing the status of the KBC
 */
kbc_status (kbc_parse_status)();

/**
 * @brief ascertains if the KBC can receive input, on a given moment
 * @return true if the KBC can receive input
 * @return false if the KBC cannot receive input
 */
bool (kbc_can_write)();

/**
 * @brief ascertains if the KBC has output ready to be read, on a given moment
 * @return true if the KBC has output
 * @return false if the KBC does not have output
 */
bool (kbc_can_read)();

/**
 * @brief delays writing to the KBC, until it is ready to receive input
 * @param wait_ticks number of attempts that will be made to write to the KBC
 * @return zero if the wait was successful and the number of attempts was not exceeded, non-zero otherwise
 */
int (kbc_delay_write)(uint32_t wait_ticks);

/**
 * @brief delays reading the KBC, until it is ready to receive input
 * @param wait_ticks number of attempts that will be made to reading the KBC
 * @return zero if the wait was successful and the number of attempts was not exceeded, non-zero otherwise
 */
int (kbc_delay_read)(uint32_t wait_ticks);

// command byte functions
/**
 * @brief writes a command to the KBC
 * @param command the command byte that will be written to the KBC
 * @param wait_ticks the number of attempts that will be made
 * @return zero on success, non-zero otherwise
 */
int (kbc_write_command)(uint8_t command, uint32_t wait_ticks);

/**
 * @brief reads the command register of the KBC
 * @param command pointer which will contain the command byte read
 * @param wait_ticks the number of attempts that will be made
 * @return zero on success, non-zero otherwise
 */
int (kbc_get_command_byte)(uint8_t* command, uint32_t wait_ticks);

/**
 * @brief writes to the command register of the KBC
 * @param command byte that will be written to the command register
 * @param wait_ticks the number of attempts that will be made
 * @return zero on success, non-zero otherwise
 */
int (kbc_set_command_byte)(uint8_t command, uint32_t wait_ticks);

// I/O buffer functions
/**
 * @brief reads the output buffer of the KBC
 * @param data pointer that will contain the content of the output buffer of the KBC
 * @param wait_ticks the number of attempts that will be made
 */
int (kbc_read_out_buf)(uint8_t* data, uint32_t wait_ticks);

/**
 * @brief writes to the output buffer of the KBC
 * @param data data that will be written to the output buffer of the KBC
 * @param wait_ticks the number of attempts that will be made
 */
int (kbc_write_in_buf)(uint8_t data, uint32_t wait_ticks);

#endif // _LCOM_KBC_H_
