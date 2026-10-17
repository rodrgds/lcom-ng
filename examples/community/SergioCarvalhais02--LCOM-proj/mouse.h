#ifndef _MOUSE_H_
#define _MOUSE_H_


#include <lcom/lcf.h>
#include <lcom/utils.h>

#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"


int mouse_hook_id, found_err, data_read_to;
uint8_t m_data;
struct packet pp;


int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

int (mouse_issue_cmd)(uint8_t mouse_cmd);

int (get_mouse_ack)(uint8_t * ack);


#endif /* _MOUSE_H_ */
