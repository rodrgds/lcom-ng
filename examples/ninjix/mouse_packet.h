#ifndef NINJIX_MOUSE_PACKET_H
#define NINJIX_MOUSE_PACKET_H

#include "mouse.h"

int mouse_process_byte(uint8_t byte);
int mouse_get_packet(struct packet *pp);
void mouse_packet_reset(void);
void mouse_apply_packet_delta(const struct packet *pp,
                              int16_t *x,
                              int16_t *y,
                              int16_t min_x,
                              int16_t max_x,
                              int16_t min_y,
                              int16_t max_y);

#endif
