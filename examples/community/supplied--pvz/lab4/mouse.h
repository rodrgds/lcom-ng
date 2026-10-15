#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

uint8_t get_mouse_byte();
bool get_mouse_error();
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
int (mouse_write_command)(uint8_t command);
void mouse_build_packet(uint8_t *packet_bytes, struct packet *pp);

#endif /* _LCOM_MOUSE_H_ */
