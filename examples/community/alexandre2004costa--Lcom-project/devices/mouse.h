#ifndef __mouse_h
#define __mouse_h


#include "kbc.h"
#include <lcom/lcf.h>

void (mouse_ih)();

void (managePackages)();

int (mouse_subscribe_int) (uint8_t *bit_no) ;

int (mouse_unsubscribe_int) ();

int (write_to_mouse)(uint8_t commandByte);

#endif

