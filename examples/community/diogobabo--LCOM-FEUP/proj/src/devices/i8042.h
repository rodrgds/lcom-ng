#ifndef _I8042_
#define _I8042_

#include <lcom/lcf.h>

#define STAT_REG 0x64
#define OUT_BUF 0x60
#define WRITE_BYTE 0xD4
#define WRITE_PORT 0x60
#define DISABLE_DATA 0xF5
#define INT  BIT(0)
#define IRQ_KB 1
#define ERROR_PARITY_TIMEOUT  (BIT(6) | BIT(7))
#define WAIT_KBC    20000
#define KBC_OBF  BIT(0)
#define KBC_AUX  BIT(5)
#define DEFSCAN 0xe0
#define ESCSCAN 0x81
#define OBF_AUX  (BIT(0) | BIT(5))
#define IRQ_MOUSE 12
#define IBF_BIT BIT(1)

// defs for packet

#define LEFT_BUTTON BIT(0)
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVERFLOW BIT(6)
#define Y_OVERFLOW BIT(7)

#define ACK 0xFA  
#define NACK 0xFE 
#define ERROR 0xFC 

#endif
