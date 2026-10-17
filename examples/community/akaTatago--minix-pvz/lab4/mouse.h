#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
int mouse_write_cmd(uint8_t cmd);
void mouse_parse_packet(struct packet *pp, uint8_t bytes[]);

int (mouse_enable_data_report)();
int (mouse_disable_data_report)();

#endif /* __MOUSE_H */
