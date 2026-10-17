//
// Created by joao on 15-04-2024.
//

#ifndef SHARED_MOUSE_H
#define SHARED_MOUSE_H
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.h"

int (mouse_subscribe_int)();

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void (mouse_sync_bytes)();

void (mouse_bytes_to_packet)();

int (mouse_write)(uint8_t command);
#endif //SHARED_MOUSE_H
