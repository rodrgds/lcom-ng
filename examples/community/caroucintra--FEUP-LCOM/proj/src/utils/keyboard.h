#include "i8254.h"
#include "i8042.h"

int kbd_hookid;
uint8_t scancode[2];
bool secondbyte;
bool make;

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

void (kbc_ih)(void);

int (kbc_read_keycode)();

int (kbc_interrupts_enable)();

int (write_kbc_reg)(uint8_t command);

