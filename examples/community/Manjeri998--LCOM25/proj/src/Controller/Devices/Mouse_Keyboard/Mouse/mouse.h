#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../../macros.h"
#include "../../func_aux.h"
#include "../kbc.h"

bool packet_complete;
struct packet p;

int (sub_mouse_int)(uint8_t* mask);
int (unsub_mouse_int)();
void (mouse_ih)();
int (write_mouse_command)(uint8_t command);
int (read_packet_byte)();

#endif 
