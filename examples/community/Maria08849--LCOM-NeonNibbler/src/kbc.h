#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include "auxfuncs.h"
#include "defines.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

void(kbc_ih)();

int(get_keyboard_data)();

int(read_KBC_status)(uint8_t *status);

int(read_KBC_output)(uint16_t *data);

int(write_KBC)(uint8_t port, uint8_t byte);

int(keyboard_subscribe_interrupts)(uint8_t *bit_no);

int(keyboard_unsubscribe_interrupts)();

int(process_and_print_keyboard_data)(uint16_t data);

/* restore keyboard command byte */
int(keyboard_restore)(uint8_t cmd);

#endif /* _LCOM_KBC_H_ */
