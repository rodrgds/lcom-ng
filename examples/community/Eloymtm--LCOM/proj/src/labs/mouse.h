#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "aux.h"
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();

void synchronize_driver();
void createPacket();


int(mouse_commu)(uint8_t command);
#endif
