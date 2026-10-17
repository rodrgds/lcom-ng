#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

// most interrupts are associated with 1 byte scancodes, ih will deal with this value changing;
int scancode_size = 1; /**< @brief the size of the scancode */
int scancode_type;     /**< @brief the type of the scancode: can be MAKE or BREAK */

int32_t kbc_global_hook_id; /**< @brief the hook id that is used to unsubscribe interrupts from the KBC */
uint8_t st_reg;             /**< @brief variable used to store the status register of the KBC */

uint8_t *scancodes = NULL; /**< @brief array of bytes that make up the scancode */

bool keyboard_packet_ready = false, /**< @brief indicates whether the keyboard packet is ready or not */
  scancode_processed = false;       /**< @brief indicates whether the current scancode has been processed or not */
int bytes_read = 0;                 /**< @brief the number of bytes from the current scancode that have been read so far */

int(kbc_subscribe_int)(uint8_t *bit_no) {
  int hook_id = KBC_KEYBOARD_IRQ;

  *bit_no = (uint8_t) hook_id;

  int res = sys_irqsetpolicy(KBC_KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);

  kbc_global_hook_id = (uint8_t) hook_id;

  return res;
}

int(kbc_unsubscribe_int)() {
  return sys_irqrmpolicy((int *) &kbc_global_hook_id);
}

void(kbc_ih)() {

  if (scancode_processed) {
    scancode_processed = false;
    scancode_size = 1; // reset scancode_size for next scancode, always at least 1
    free(scancodes);
    scancodes = NULL;
  }

  uint8_t scancode_byte;
  keyboard_packet_ready = false;

  util_sys_inb(KBC_OUT_BUF_ST, &st_reg);     // query status register
  util_sys_inb(KBC_OUT_BUF, &scancode_byte); // safe to read, KBC triggers interrupt on OUT_BUFF full
  bytes_read++;

  if (st_reg & (KBC_ST_TIMEOUT | KBC_ST_PARITY)) { // if timeout/transmission error, abort current interrupt
    bytes_read = 0;
    scancode_size = 0;
    return;
  }

  if (scancode_byte == KBC_SCANCODE_DOUBLE_BYTE || scancode_byte == KBC_SCANCODE_TRIPLE_BYTE) { // mark that we have to read more bytes
    scancode_size += (uint8_t) (scancode_byte & 0xF) + 1;                                       // declare that we will need to read more bytes
  }

  scancodes = realloc(scancodes, sizeof(uint8_t) * bytes_read);

  if (!scancodes) { // in case realloc returns NULL -> error
    bytes_read = 0;
    scancode_size = 0;
    return;
  };

  scancodes[bytes_read - 1] = scancode_byte;

  if (bytes_read == scancode_size) {
    keyboard_packet_ready = true; // mark scancode fully read
    bytes_read = 0;               // reset bytes read for next scancode
    scancode_type = (scancodes[scancode_size - 1] & BIT(7)) ? KBC_SCANCODE_BREAK : KBC_SCANCODE_MAKE;
  }
}

void(kbc_enable_int)() {

  uint8_t status_reg;

  do {
    status_reg = 0;
    util_sys_inb(KBC_OUT_BUF_ST, &status_reg);
  } while (status_reg & KBC_ST_IBF);

  // input buffer no longer full, safe to write commands

  sys_outb(KBC_IN_BUF_CMD, KBC_RD_CMD);

  uint8_t command_byte;
  util_sys_inb(KBC_OUT_BUF, &command_byte);

  command_byte |= KBC_INT;

  sys_outb(KBC_IN_BUF_CMD, KBC_WR_CMD);

  sys_outb(KBC_IN_BUF_ARG, command_byte);
}

int(kbc_outbuf_full)() {
  st_reg = 0;
  util_sys_inb(KBC_OUT_BUF_ST, &st_reg);

  return (st_reg & KBC_ST_OBF);
}

bool(keyboard_ready)() {
  return keyboard_packet_ready;
}

void(mark_scancode_processed)() {
  scancode_processed = true;
}

int(get_scancode_size)() {
  return scancode_size;
}

int(get_scancode_type)() {
  return scancode_type;
}
