#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include "../i8042.h"
#include <stdbool.h>
#include <stdint.h>

extern int keyboard_hook_id;
extern uint8_t scancode;

void keyboard_ih();

int keyboard_subscribe_int(uint8_t *bit_no);

int keyboard_unsubscribe_int ();

int restore_interruptions();

bool is_makecode(uint8_t scancode);

int code_size(uint8_t scancode);

#endif

