#include "keyboard.h"

uint8_t scancode = 0;
int keyboard_hook_id = 1;

// interrupt handler for keyboard
void(kbc_ih)() {
  if ((KBC_read_output)(KBC_OUT_CMD, &scancode, 0) != 0) {
    printf("[ERROR] could not read scancode\n");
  }
}

int keyboard_subscribe_interrupts(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;                      // check for null pointer
  *bit_no = BIT(keyboard_hook_id); // set bit_no with hook id bit
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
                          &keyboard_hook_id); // set irq policy
}

int keyboard_unsubscribe_interrupts() {
  return sys_irqrmpolicy(&keyboard_hook_id); // remove irq policy
}

int keyboard_restore() {
  uint8_t commandByte;

  // read the current command byte from the keyboard controller
  if (KBC_write_command(KBC_IN_CMD, KBC_READ_CMD) != 0)
    return 1;

  // retrieve the command byte from the output buffer
  if (KBC_read_output(KBC_OUT_CMD, &commandByte, 0) != 0)
    return 1;

  // enable keyboard interrupts by setting the appropriate bit
  commandByte = commandByte | ENABLE_INT;

  // prepare to write the modified command byte back to the controller
  if (KBC_write_command(KBC_IN_CMD, KBC_OUT_CMD) != 0)
    return 1;

  // write the modified command byte to the controller
  if (KBC_write_command(KBC_OUT_CMD, commandByte) != 0)
    return 1;

  return 0;
}

char scancode_to_char(uint8_t scancode){
    if (scancode == 0x1E) return 'A';
    if (scancode == 0x30) return 'B';
    if (scancode == 0x2E) return 'C';
    if (scancode == 0x20) return 'D';
    if (scancode == 0x12) return 'E';
    if (scancode == 0x21) return 'F';
    if (scancode == 0x22) return 'G';
    if (scancode == 0x23) return 'H';
    if (scancode == 0x17) return 'I';
    if (scancode == 0x24) return 'J';
    if (scancode == 0x25) return 'K';
    if (scancode == 0x26) return 'L';
    if (scancode == 0x32) return 'M';
    if (scancode == 0x31) return 'N';
    if (scancode == 0x18) return 'O';
    if (scancode == 0x19) return 'P';
    if (scancode == 0x10) return 'Q';
    if (scancode == 0x13) return 'R';
    if (scancode == 0x1F) return 'S';
    if (scancode == 0x14) return 'T';
    if (scancode == 0x16) return 'U';
    if (scancode == 0x2F) return 'V';
    if (scancode == 0x11) return 'W';
    if (scancode == 0x2D) return 'X';
    if (scancode == 0x15) return 'Y';
    if (scancode == 0x2C) return 'Z';
    if (scancode == 0x02) return '1';
    if (scancode == 0x03) return '2'; 
    if (scancode == 0x04) return '3'; 
    if (scancode == 0x05) return '4'; 
    if (scancode == 0x06) return '5'; 
    if (scancode == 0x07) return '6'; 
    if (scancode == 0x08) return '7'; 
    if (scancode == 0x09) return '8'; 
    if (scancode == 0x0A) return '9'; 
    if (scancode == 0x0B) return '0';  

    if (scancode == 0x48) return '^';   // Up arrow
    if (scancode == 0x50) return 'v';   // Down arrow
    if (scancode == 0x4B) return '<';   // Left arrow
    if (scancode == 0x4D) return '>';   // Right arrow

    if (scancode == 0x39) return ' ';   // Space
    if (scancode == 0x1C) return '\n';  // Enter

    return '\0';
}
