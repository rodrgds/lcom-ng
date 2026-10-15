#ifndef __keyboard_h
#define __keyboard_h


#include "kbc.h"

void (kbd_ih)();

int (enableInterrupt)();

int (package_KScanCode)();

void (waitForEsc)();

int (keyboard_subscribe_int) (uint8_t *bit_no);

int (keyboard_unsubscribe_int) ();


#endif
