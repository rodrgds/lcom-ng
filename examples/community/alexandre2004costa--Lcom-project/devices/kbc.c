#include "kbc.h"

/** @defgroup kbc kbc
 * @{
 */

#define DELAY_US 20000

/**
 * @brief Reads the KBC status register
 * @param status Pointer to store the status register value
 * @return 0 on success, 1 otherwise
 */
int (read_KBC_status)(uint8_t* status){
  return util_sys_inb(STATUS_REG, status);
}

/**
 * @brief Reads the KBC output buffer
 * @param port Port to read from
 * @param output Pointer to store the output value
 * @param isMouse Boolean indicating if the output is from a mouse
 * @return 0 on success, 1 otherwise
 */
int (read_KBC_output)(uint8_t port, uint8_t *output, bool isMouse){
  uint8_t k = 10;
  while (k > 0)
  {
    uint8_t status;
    if (read_KBC_status(&status) != 0 || status & ST_TIME_OUT_ERROR || 
    status & ST_PARITY_ERROR){
      printf("Error in status");
      return 1;
    }
    if (status & ST_OUT_FULL_BUFFER){
      if ((isMouse && (status & ST_MOUSE_DATA)) || (!isMouse && (!(status & ST_MOUSE_DATA)))){
        if (util_sys_inb(OUT_KBC_CMD, output) != 0) return 1;
        return 0;
      } 
    }
    tickdelay(micros_to_ticks(DELAY_US));
    k--;
  }
  return 0;
}

/**
 * @brief Writes a command byte to the KBC
 * @param port Port to write to
 * @param commandByte Command byte to write
 * @return 0 on success, 1 otherwise
 */
int (write_KBC_command)(uint8_t port, uint8_t commandByte){
  uint8_t k = 10;
  while (k > 0)
  {
    uint8_t status;
    if (read_KBC_status(&status) != 0 || status & ST_TIME_OUT_ERROR || 
    status & ST_PARITY_ERROR || status & ST_MOUSE_DATA){
      printf("Error in status");
      return 1;
    }
    if (!(status & ST_IN_FULL_BUFFER)){
      if (sys_outb(port, commandByte) != 0) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    k--;
  }
  return 0;
}

/**@}*/

