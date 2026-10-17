#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../../macros.h"
#include "../../func_aux.h"
#include "../kbc.h"

uint8_t scan_code;

void (kbc_ih)();
int (get_scancode)();
int (kb_sub_int)(uint8_t *mask);
int (kb_unsub_int)();

#endif
