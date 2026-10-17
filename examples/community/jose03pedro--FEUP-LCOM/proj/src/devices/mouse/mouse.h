#ifndef _SRC_MOUSE_H_
#define _SRC_MOUSE_H_

#define MOUSE_MASK BIT(2)

#include "KBC.h"
#include "devices/video/graphics.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

typedef struct {
  uint8_t right_click;
  uint8_t left_click;
  int16_t x;
  int16_t y;
} MouseInfo;

int(mouse_subscribe_interrupts)();

int(mouse_unsubscribe_interrupts)();

void(mouse_ih)();

void(mouse_sync_bytes)();

void(mouse_sync_info)();

int(mouse_write)(uint8_t cmd);

#endif
