#ifndef _MOUSE_H
#define _MOUSE_H

#include <minix/syslib.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
int (mouse_issue_cmd)(uint8_t cmd);

void (mouse_ih)(void);

void (mouse_place_byte)(void);
void (mouse_create_packet)(void);

#endif
