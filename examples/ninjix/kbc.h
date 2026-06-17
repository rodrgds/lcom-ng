#ifndef NINJIX_KBC_H
#define NINJIX_KBC_H

#include "ninjix_platform.h"

#include <lcom/i8042.h>

#define KBC_ST_TIMEOUT BIT(6)
#define KBC_ST_PARITY BIT(7)
#define KBC_ST_ERR (KBC_ST_PARITY | KBC_ST_TIMEOUT)
#define KBC_CB_INT BIT(0)
#define KBC_CB_INT2 BIT(1)

int kbc_subscribe_int(uint8_t *bit_no);
int kbc_unsubscribe_int(void);
void kbc_ih(void);
int kbc_get_byte(uint8_t *byte);
bool kbc_had_error(void);
int kbc_read_output(uint8_t *data);
int kbc_issue_cmd(uint8_t cmd);
int kbc_write_arg(uint8_t arg);
int kbc_read_cmd_byte(uint8_t *cb);
int kbc_write_cmd_byte(uint8_t cb);
int kbc_enable_int(void);
int kbc_disable_int(void);

#endif
