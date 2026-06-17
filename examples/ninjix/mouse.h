#ifndef NINJIX_MOUSE_H
#define NINJIX_MOUSE_H

#include "ninjix_platform.h"

#include <lcom/i8042.h>

#define MOUSE_LB_BIT 0
#define MOUSE_RB_BIT 1
#define MOUSE_MB_BIT 2
#define MOUSE_X_OV_BIT 6
#define MOUSE_Y_OV_BIT 7
#define MOUSE_DELTA_X_SIGN_BIT 4
#define MOUSE_DELTA_Y_SIGN_BIT 5
#define MOUSE_SIGN_EXTEND_MASK 0xFF00

int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int(void);
void mouse_ih(void);
int mouse_get_byte(uint8_t *byte);
bool mouse_had_error(void);
int mouse_send_cmd(uint8_t cmd);
int mouse_enable_data_report(void);
int mouse_disable_data_report(void);

#endif
