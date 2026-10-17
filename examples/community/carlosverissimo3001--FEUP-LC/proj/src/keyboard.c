#include <lcom/lcf.h>
#include "keyboard.h"

#include "macros.h"
#include <stdint.h>

int keyboard_hook_id = KBC_IRQ;
int is_done = 0;
int kbc_done = 0;

static int is_two_byte;
static int scancode_size = 1;
static uint8_t scancode[2]; // Contains the scancode of the key
static int error;           // error flag

int(kbc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = keyboard_hook_id;

  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != OK) {
    printf("Error while subscribing the keyboard interruptions.\n");
    return 1;
  }

  return 0;
}

int(kbc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&keyboard_hook_id) != OK) {
    printf("Error while unsubscribing the keyboard interruptions.\n");
    return 1;
  }

  return 0;
}

void(kbc_ih)() {
  uint8_t st;

  // Read the kbc status
  if (kbc_get_status(&st) != OK) {
    error = true; // if an error has occured, set error = true;
    return;
  }

  // Check for timeout and parity errors
  if (st & KBC_CHECK_ERR)
    error = true; // if an error has occured, set error = true;

  uint8_t buffer;
  // Read the byte from the output buffer
  if (kbc_read_out_buf(&buffer) != OK) {
    error = true; // if an error has occured, set error = true;
    return;
  }

  kbc_populate_buff(buffer);
  return;
}

int(kbc_get_status)(uint8_t *st) {
  return util_sys_inb(KBC_ST_REG, st);
}

int(kbc_set_command_byte)(uint8_t cmdByte) {
  if (kbc_write_command(KBC_ST_REG, KBC_WRITE_CMD_BYTE))
    return 1;
  if (kbc_write_command(KBC_IN_BUF, cmdByte))
    return 1;
  return 0;
}

int(kbc_get_command_byte)(uint8_t *cmdByte) {
  if (kbc_write_command(KBC_ST_REG, KBC_READ_CMD_BYTE))
    return 1;

  if (kbc_read_data(cmdByte))
    return 1;

  return 0;
}

int(kbc_read_out_buf)(uint8_t *buffer) {
  if (util_sys_inb(KBC_OUT_BUF, buffer) != OK) {
    printf("Error while reading from the output buffer\n");
    return 1;
  }
  return 0;
}

int(kbc_populate_buff)(uint8_t buffer) {
  // check if we read the 0xe0.
  if (buffer == TWO_BYTE_SCODE) {
    scancode[0] = buffer;
    is_two_byte = 1; // if so, we set the twoByte flag and return
    return 0;
  }

  if (is_two_byte) // if the flag was previously set
  {
    is_two_byte = 0;      // unset it
    scancode[1] = buffer; // the second byte of the array is gonna be the buffer we just read
  }

  else { // otherwise, the first byte is the scancode we read
    scancode[0] = buffer;
  }

  return 0;
}

int(is_make_code)() {
  return !(scancode[scancode_size - 1] & BIT(7));
}

int(kbc_polling)() {
  uint8_t buffer;

  if (kbc_read_data(&buffer))
    return 1;

  // nunca chega aqui !!!!!
  //printf("Successfully polled data from keyboard\n");

  kbc_populate_buff(buffer);

  return 0;
}

int(kbc_write_command)(uint8_t port, uint8_t command) {
  uint8_t st;
  int tries = 0;

  while (true) {
    kbc_get_status(&st);

    /* loop while 8042 input buffer is not empty */
    if (!(st & KBC_IBFULL)) {
      sys_outb(port, command); /* no args command */
      return 0;
    }
    tries++;
    if (tries > MAX_TRIES)
      return 1;

    delay(); // e.g. tickdelay()
  }

  return 0;
}

int(kbc_read_data)(uint8_t *data) {
  uint8_t st;

  //delay();

  if (kbc_get_status(&st)){
    printf("Erorr while reading status\n");
    return 1;
  }
  if (!(st & KBC_OBFULL) || (st & KBC_MOUSE_DATA)){
    //printf("Output buffer is not full or data is from mouse\n");
    return 1;
  }

  if (kbc_read_out_buf(data))
    return 1;

  if (st & KBC_CHECK_ERR)
    error = true;

  return 0;
}

void delay() {
  tickdelay(micros_to_ticks(KBC_DELAY));
}

/**
 * GETTERS AND SETTERS FOR GLOBAL VARIABLES
 * /
 *
 */

uint8_t *(get_scancode) () {
  return scancode;
}

int(is_error)() {
  return error;
}

int(get_scancode_size)() {
  return scancode_size;
}

void(set_scancode_size)(int size) {
  scancode_size = size;
}

int(two_byte_code)() {
  return is_two_byte;
}
