#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include "scancodes_set1.h"

int kbd_subscribe_int(uint8_t *bit_no);
int kbd_unsubscribe_int();
void (kbc_ih)();
int (kbc_write_command)(uint8_t cmd);
int (kbc_write_argument)(uint8_t arg);
int (kbc_read_output)(uint8_t *output);

extern uint8_t scancode[2];
extern uint8_t scancode_size;
extern bool error;
extern uint32_t sys_inb_cnt;
extern bool scancode_ready;

#endif

