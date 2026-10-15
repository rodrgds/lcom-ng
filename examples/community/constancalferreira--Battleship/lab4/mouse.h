#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdbool.h>

#include "i8042.h"
#include "KBC.h"



int (mouse_subscribe_int) (uint32_t* irq_set);
int (mouse_unsubscribe_int)();
void (mouse_ih)();

int (mouse_write_cmd)(uint8_t command);

int (_mouse_enable_data_reporting)();
int (_mouse_disable_data_reporting)();

bool (mouse_sync_bytes)();

void (create_packet)(struct packet* mouse_packet);


#endif /* _MOUSE_H_ */
