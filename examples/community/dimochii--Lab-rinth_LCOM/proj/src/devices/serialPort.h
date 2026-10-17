#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_
#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "SerialPorts.h"
#include "tools/queue.h"
#include "../game/Game.h"
#include "i8042.h"

int (sp_subscribe_int)(uint8_t* irq_set);
int (sp_unsubscribe_int)();

bool (sp_enable_int)();
bool (sp_disable_int)();

int (sp_initialize)();

int (receive_byte)();

int (send_byte)(uint8_t byte);
int (send_queue_bytes)();

void (send_scan)(uint8_t scancode);
void (manage_button)(uint8_t scancode, bool isPlayer1);

int (sp_clean_int)();
void (sp_ih)();

void (sp_handle_start_multi)();
void (sp_handle_receive_info)();

void (sp_handler)();

#endif
