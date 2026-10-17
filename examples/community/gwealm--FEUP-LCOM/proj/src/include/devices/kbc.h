#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#define KBC_DELAY_US 20000
#define KBC_TIMEOUT_TRIES 5

#define KBC_ACK 0xFA
#define KBC_NACK 0xFE
#define KBC_ACK_ERROR 0xFC

#define KBC_REG_STAT 0x64
#define KBC_REG_OUT_BUF 0x60

#define KBC_REG_CTRL 0x64
#define KBC_REG_IN_BUF 0x60

#define KBC_STAT_PARITY_ERR BIT(7)
#define KBC_STAT_TIMEOUT_ERR BIT(6)
#define KBC_STAT_AUX BIT(5)
#define KBC_STAT_KB_INH BIT(4)
#define KBC_STAT_A2 BIT(3)
#define KBC_STAT_SYS BIT(2)
#define KBC_STAT_IBF BIT(1)
#define KBC_STAT_OBF BIT(0)

enum kbc_command_dest {
  MOUSE = 0xD4
} kbc_command_dest_t;

struct kbc_status {
  bool parity_error;
  bool timeout_error;
  bool aux;
  bool keyboard_inhibited;
  bool a2;
  bool sys;
  bool input_buffer_full;
  bool output_buffer_full;
};

/**
 * @brief Reads KBC status
 *
 * @return 0 if reading was successful, non-zero otherwise
 */
int (kbc_read_status)(struct kbc_status *stat);

/**
 * @brief Sends a byte to a KBC port
 * 
 * @param port Either KBC_REG_CTRL or KBC_REG_IN_BUF
 * @param byte The byte to send
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (kbc_send_byte)(int port, uint8_t byte);

/**
 * @brief Reads a byte from a KBC port
 *
 * @param port Either KBC_REG_STAT or KBC_REG_OUT_BUF
 * @param byte Where to store the information
 * @return 0 if reading was successful, non-zero otherwise
 */
int (kbc_read_byte)(int port, uint8_t *byte);

/**
 * @brief Sends a command to a device
 *
 * @param dest The destination device
 * @param bytes The bytes to send
 * @param n The number of bytes in the command
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (kbc_send_command)(enum kbc_command_dest dest, uint8_t *bytes, size_t n);
