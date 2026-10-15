#include <lcom/lcf.h>
#include "kbc.h"
#include "i8042.h"

int hook_id_kb;
static uint8_t scancode;
static bool kbc_error = false;

static bool is_two_byte = false;
static uint8_t full_scancode[2] = {0};
static int scancode_size = 0;
static bool make = false;

static bool ready = false;

static const char scancode_to_ascii[] = {
  0,    0,   '1', '2', '3', '4', '5', '6',  // 0x00-0x07
  '7', '8', '9', '0', '0', 0,  0, 0,        // 0x08-0x0F
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',   // 0x10-0x17
  'o', 'p', 0, 0, 0, 0,  'a', 's',          // 0x18-0x1F
  'd', 'f', 'g', 'h', 'j', 'k', 'l', 0,     // 0x20-0x27
  0,  0,  0,  0,'z', 'x', 'c', 'v',         // 0x28-0x2F
  'b', 'n', 'm', 0, 0, 0,  0,    0,         // 0x30-0x37
};

int(kbc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  hook_id_kb = KBC_IRQ;
  *bit_no = hook_id_kb;
  if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kb)) return 1;
  return 0;
}

int(kbc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_kb)) return 1;
  return 0;
}

void (kbc_ih)() {
  uint8_t sr; 
  kbc_error = true;

  /* Read the status register */
  if (util_sys_inb(KBC_STAT_REG, &sr)) {
    kbc_error = true;
    return;
  }

  /* Leave mouse bytes in the shared buffer for the mouse handler. */
  if (!(sr & OBF) || (sr & MOUSE_DATA)) return;

  /* Read the scancode from the output buffer */  
  if (util_sys_inb(KBC_OUT_BUF, &scancode)) {
    kbc_error = true;
    return;
  }
  /* Bad keyboard bytes have been consumed but must not become scancodes. */
  kbc_error = (sr & (PARITY_ERR | TIMEOUT_ERR)) != 0;
}

// ------------ GETTERS ------------

uint8_t (kbc_get_scancode)() {
  return scancode;
}

uint8_t (kbc_has_error)() {
  return kbc_error;
}

// ------------ POLLING FUNCTIONS ------------

int (kbc_read_status)(uint8_t* status) {
  if (status == NULL) return 1; 
  if (util_sys_inb(KBC_STAT_REG, status)) return 1;
  return 0;
}

int (kbc_read_output)() {

  uint8_t status;
  if (kbc_read_status(&status)) return 1;

  //Check if output buffer is full. If so, we can read from it.
  if ((status & OBF)) {
      if(util_sys_inb(KBC_OUT_BUF, &scancode))   return 1;
      if((status & PARITY_ERR)) return 1;
      if((status & TIMEOUT_ERR)) return 1;
      return 0;
  }

  return 1;
}

int (kbc_read_command)(uint8_t *command) {

  if (command == NULL) return 1;

  uint8_t status;
  uint8_t attempts = KBC_MAX_ATTEMPTS;
  
  while (attempts) {

      if (kbc_read_status(&status)) return 1;

      //Check if output buffer is full. If so, we can read from it.
      if ((status & OBF)) {
          if(util_sys_inb(KBC_OUT_BUF, command))   return 1;
          if((status & PARITY_ERR)) return 1;
          if((status & TIMEOUT_ERR)) return 1;
          return 0;
      }

      tickdelay(micros_to_ticks(KBC_DELAY_US));
      attempts--;
  }

  return 1;
}

int (kbc_write_command)(uint8_t port, uint8_t commandByte) {

  uint8_t status;
  uint8_t attempts = KBC_MAX_ATTEMPTS;

  while (attempts) {

      if (kbc_read_status(&status) != 0) return 1;

      //Check if input buffer is empty. If so, we can write to it.
      if ((status & IBF) == 0){
          if(sys_outb(port, commandByte)) return 1;
          return 0;
      }

      tickdelay(micros_to_ticks(KBC_DELAY_US));
      attempts--;
  }

  return 1;
}

int (kbc_int_enable)() {

  //Reads the corrent command setting
  uint8_t commandWord;
  if (kbc_write_command(KBC_CMD_REG, 0x20)) return 1;
  if (kbc_read_command(&commandWord)) return 1;

  //Writes the enabling command
  commandWord = commandWord | BIT(0);
  if (kbc_write_command(KBC_CMD_REG, KBC_IN_BUF)) return 1;
  if (kbc_write_command(KBC_IN_BUF, commandWord)) return 1;

  return 0;
}

// PROJETO --------------------------------------------

int keyboard_init(uint8_t *bit_no) {
  if(kbc_subscribe_int(bit_no)) return 1;
  return 0;
}

int keyboard_cleanup(void) {
  if(kbc_unsubscribe_int()) return 1;
  return 0;
}

void keyboard_ih(void) {
  ready = false;
  kbc_ih();

  if(kbc_has_error()) {
    return;
  }

  if(is_two_byte) {
    full_scancode[1] = kbc_get_scancode();
    scancode_size = 2;
    ready = true;
    is_two_byte = false;
  }
  else {
    full_scancode[0] = kbc_get_scancode();
    if (full_scancode[0] == 0xE0) {
      is_two_byte = true;
      ready = false;
      return;
    }
    scancode_size = 1;
    ready = true;
  }

  make = !(full_scancode[scancode_size - 1] & BIT(7));
}

bool keyboard_get_scancode(uint8_t *scancode) {
  if (ready) {
    for (int i = 0; i < scancode_size; i++) {
      scancode[i] = full_scancode[i];
  }
  }

  return ready;
}

bool keyboard_is_scancode_make() {
  return make;
}

char keyboard_scancode_to_ascii() {
  if (!ready) return 0;
  if(scancode_size == 2) return 0;

  uint8_t code = full_scancode[0];
  if (code >= sizeof(scancode_to_ascii)) return 0;

  return scancode_to_ascii[code];
}
