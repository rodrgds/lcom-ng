#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "i8042.h"
#include <lcom/lcf.h>
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <stdint.h>

int kbc_subscribe_int(uint8_t *bit_no);
int kbc_unsubscribe_int();
int kbc_read_cmd(uint8_t *data);
int kbc_read_status(uint8_t *status);
int get_kbc_data(uint8_t *data);
int get_kbc_error();
int kbc_write_cmd(uint8_t command);
int get_flag(bool *flag);


#endif // KEYBOARD_H
