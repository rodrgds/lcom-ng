#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include "i8042.h"    
#include "KBC.h"      
#include <stdbool.h>

#define MOUSE_IRQ    12
#define MOUSE_CMD_WRITE             0xD4
#define MOUSE_ENABLE_DATA_REPORTING 0xF4
#define MOUSE_DISABLE_DATA_REPORTING 0xF5

#define MOUSE_LEFT_BUTTON   BIT(0)
#define MOUSE_RIGHT_BUTTON  BIT(1)
#define MOUSE_MIDDLE_BUTTON BIT(2)
#define MOUSE_X_OVERFLOW    BIT(6)
#define MOUSE_Y_OVERFLOW    BIT(7)
#define MOUSE_MSB_X_DELTA   BIT(4)
#define MOUSE_MSB_Y_DELTA   BIT(5)

typedef struct {
  int16_t dx, dy;
  bool lb, mb, rb;
} MouseEvent;

extern MouseEvent mouse_event;
extern bool packet_ready;

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)(void);
int (mouse_enable_data_report)(void);
int (mouse_disable_data_report)(void);
void (mouse_ih)(void);

#endif
