#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KBD_IRQ 1

#define OUT_BUF 0x60
#define WRITE_CMD 0x60
#define STAT_IN 0x64
#define IN_CMD 0x64

#define CMD_REG 0x64
#define READ_CMD 0x20
#define READ_CMD_VAL 0x60
#define DELAY_US    20000

#define MOUSE_IRQ 12


#endif

