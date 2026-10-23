#ifndef GRUPO_2LEIC04_5_MOUSE_H
#define GRUPO_2LEIC04_5_MOUSE_H
#include <lcom/lcf.h>
int my_mouse_enable_data_reporting(void);
int my_mouse_disable_data_reporting(void);
int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int(void);
bool mouse_packet_ready(void);
void mouse_parse_packet(struct packet *packet);
void mouse_clear_packet_flag(void);
#endif
