#include "KBC.h"
#include <lcom/lcf.h>
#include <stdint.h>

/*
 7 | Parity  | Parity error - invaliddata
 6 | Timeout | Timeout error - invaliddata
 5 | Aux     | Mouse data
 4 | INH     | Inhibit flag: 0 if keyboard is inhibited
 3 | A2      | A2 input line: irrelevant for LCOM
 2 | SYS     | System flag: irrelevant for LCOM
 1 | IBF     | Input buffer full don’t write commands or arguments
 0 | OBF     | Output buffer full - data available for reading
 */
int(kbc_check_config)(uint8_t *st) {
  uint8_t status;

  if (util_sys_inb(KBC_STATUS_REG, &status) != 0) {
    printf("Failed to read status register\n");
    return 1;
  }

  *st = status;
  return 0;
}


int(kbc_read_data)(uint8_t port, uint8_t *data) {
  int attempts = 0;
  uint8_t status = 0;

  while (MAX_ATTEMPTS > attempts) {
    // Check if there was some communications error and read the status register
    if (kbc_check_config(&status) != 0) {
      printf("Failed to check configuration\n");
      return 1;
    }

    // OBF has data to read
    if ((status & KBC_OBF) != 0) {
      // Read data from the output buffer
      if (util_sys_inb(port, data) != 0) {
        printf("Failed to read status register\n");
        return 1;
      }

      if (status & KBC_PARITY) {
        printf("Parity error - invalid data\n");
        return 1;
      }

      if (status & KBC_TIMEOUT) {
        printf("Timeout error - invalid data\n");
        return 1;
      }

      return 0;
    }

    attempts++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}


int(kbc_write_command)(kbc_commands_t command, uint8_t *command_byte) {
  uint8_t status;
  int attempts = 0;

  while (attempts < MAX_ATTEMPTS) {
    if (kbc_check_config(&status) != 0) {
      printf("Failed to check configuration\n");
      return 1;
    }

    if (status & KBC_PARITY) {
      printf("Parity error - invalid data\n");
      return 1;
    }

    if (status & KBC_TIMEOUT) {
      printf("Timeout error - invalid data\n");
      return 1;
    }

    // Check if the input buffer is empty
    if ((status & KBC_IBF) == 0) {
      if (command != KBC_CUSTOM_CMD) { // If the command is not custom, write the command to the KBC
        if (sys_outb(KBC_CMD_REG, (uint8_t) command) != 0) {
          printf("Failed to write command to KBC\n");
          return 1;
        }
      }
      else { // If the command is custom, write the command byte to the KBC
        if (sys_outb(KBC_CMD_REG, *command_byte) != 0) {
          printf("Failed to write command byte to KBC\n");
          return 1;
        }
      }
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  }

  printf("Timeout waiting for input buffer to be empty\n");
  return 1;
}

/*
 Command Byte Format
 | 7 | 6 | 5    | 4   | 3 | 2 | 1    | 0   |
 | – | – | DIS2 | DIS | – | – | INT2 | INT |

 DIS2 1: disable mouse interface
 DIS 1: disable keyboard interface
 INT2 1: enable interrupt on OBF, from mouse
 INT 1: enable interrupt on OBF, from keyboard
 - : Either not used or not relevant for Lab

 Read Use KBC command 0x20, which must be written to 0x64
 ▶ But the value of the “command byte” must be read from 0x60

 Write Use KBC command 0x60, which must be written to 0x64
 ▶ But the new value of the “command byte” must be written to 0x60

 */
int(kbc_command_byte_IO)(kbc_command_byte_components_t components[], size_t size) {
  // Say to the KBC that we want to read the command byte
  if (kbc_write_command(KBC_READ_CMD_BYTE, 0) != 0) {
    printf("Failed to enable keyboard interface\n");
    return 1;
  }

  // Read the command byte
  uint8_t command_byte = 0;
  if (kbc_read_data(KBC_CMD_IO, &command_byte) != 0) {
    printf("Failed to read data\n");
    return 1;
  }

  // Modify the command byte based on the components array
  for (size_t i = 0; i < size; i++) {
    command_byte |= components[i];
  }

  // Say to the KBC that we want to write the modified command byte
  if (kbc_write_command(KBC_WRITE_CMD_BYTE, 0) != 0) {
    printf("Failed to write command byte\n");
    return 1;
  }

  // Write the modified command byte back to the KBC
  if (kbc_write_command(KBC_CMD_IO, &command_byte) != 0) {
    printf("Failed to write command byte\n");
    return 1;
  }

  return 0;
}
