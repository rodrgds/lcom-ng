#ifndef PVZ_MOUSE_H
#define PVZ_MOUSE_H

#include "pvz_platform.h"

#include <lcom/i8042.h>

#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define MOUSE_X_SIGN BIT(4)
#define MOUSE_Y_SIGN BIT(5)
#define MOUSE_X_OVFL BIT(6)
#define MOUSE_Y_OVFL BIT(7)

uint8_t get_mouse_byte(void);
bool get_mouse_error(void);
int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int(void);
void mouse_ih(void);
int mouse_write_command(uint8_t command);
void mouse_build_packet(uint8_t *packet_bytes, struct packet *pp);

#endif
