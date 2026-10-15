#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>

extern int byte_tracking; 
extern uint8_t data;

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
void (mouse_parse_packet)(struct packet *p);

int (mouse_write_cmd)(uint8_t cmd);
int (my_mouse_enable_data_reporting)(void); 
int (my_mouse_disable_data_reporting)(void);
int (mouse_disable_int)();
int (mouse_enable_int)();
#endif