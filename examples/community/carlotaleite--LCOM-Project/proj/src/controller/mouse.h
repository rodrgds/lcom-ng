#pragma once

#include "lcom/lcf.h"
#include "i8042.h"

extern int mouse_hook_id;
extern uint8_t mouse_byte;

int(mouse_subscribe_int)(uint8_t *bit_no);

int(mouse_unsubscribe_int)();

void(mouse_ih)(void);

void(mouse_byte_sync)(uint8_t *byte_counter, uint8_t packet[]);

int (mouse_parse_packet)(uint8_t packet[], struct packet *pp);

int (mouse_write_command)(uint8_t command);
