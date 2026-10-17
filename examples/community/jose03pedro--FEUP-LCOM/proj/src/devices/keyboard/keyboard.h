#ifndef _SRC_KEYBOARD_H_
#define _SRC_KEYBOARD_H_

#define KEYBOARD_MASK BIT(1)

#include "KBC.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

int(keyboard_subscribe_interrupts)();

int(keyboard_unsubscribe_interrupts)();

void(kbc_ih)();

int(keyboard_restore)();

#endif
