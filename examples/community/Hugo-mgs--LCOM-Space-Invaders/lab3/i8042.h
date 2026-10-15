#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KBD_IRQ 1

#define BREAK_ESC 0x81

#define DELAY_US 20000

#define KBD_OUT_BUF 0x60
#define KBD_IN_BUF 0x64
#define KBD_STAT_REG 0x64
#define KBD_CMD_REG 0x64

#define KBC_RESET 0x60

#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define FULL_IN_BUF BIT(1)
#define FULL_OUT_BUF BIT(0)

#endif
