#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include "i8042.h"
#include <lcom/lab4.h>

int mouse_hookid;
uint8_t bytes[3];
extern int byte_no;
struct packet packet;
extern bool pack_done;
extern uint16_t x;
extern uint16_t y;



int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int enable_data_reporting();

int disable_data_reporting();

int (get_answer)(uint8_t * answer);

int (write_kbc_mouse_reg)(uint8_t port, uint8_t command);

int (handle_byte)();

int handle_mouse_package();


#endif /* _LCOM_MOUSE_H */
