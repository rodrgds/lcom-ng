#include <lcom/lcf.h>

#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"

int keyboard_hook_id = 1;
extern int cnt;
uint8_t scancode;

int (keyboard_rstr)() {
    if (write_command(KBC_IN_BUF, KBC_READ_CMD) != 0) return 1; // Avisar que vamos ler 
    uint8_t command;
    if (read_output(KBC_OUT_BUF, &command) != 0) return 1;
    command |= ENABLE_INTERRUPTS;
    if (write_command(KBC_IN_BUF, KBC_WRITE_CMD) != 0) return 1;
	return write_command(KBC_WRITE_CMD, command);
}

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0) return 1;
    return 0;
}

int (keyboard_unsubscribe_int)() {
    if (sys_irqrmpolicy(&keyboard_hook_id) != 0) return 1;
    return 0;
}

void (kbc_ih)() {
    read_output(KBC_OUT_BUF, &scancode);
}

