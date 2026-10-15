#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"

int mouse_send_command(uint8_t cmd);
int mouse_send_command_with_sync(uint8_t cmd);

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)(void);

void (mouse_ih)(void);

int process_packet(uint8_t byte);
bool packet_complete(void);
void reset_packet(void);
struct packet *get_packet(void);

#endif
