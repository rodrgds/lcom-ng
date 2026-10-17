#ifndef _MACROS_H_
#define _MACROS_H_

#include <lcom/lcf.h>

#define CONTROL_REG 0x43
#define TIMER_0 0x40
#define MIN_FREQ 19
#define CLOCK_FREQ 1193182
#define ESC_BREAK 0x81
#define KBD_IRQ 1
#define KBC_ST_REG 0x64
#define KBC_IN_BUF 0x60
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR BIT(6)
#define KBC_OBF BIT(0)
#define BREAK_CODE BIT(7)
#define KBC_IBF BIT(1)
#define RCB 0x20
#define WCB 0x60
#define MOUSE_IRQ 12
#define WB_TO_MOUSE 0xD4
#define ENABLE_DR 0xF4
#define DISABLE_DR 0xF5
#define MOUSE_DATA BIT(5)
#define ACK 0xFA
#define ENABLE_DR 0xF4
#define BIO_CALL 0x4F
#define VBE_MODE 0x02
#define LINEAR_FB BIT(14)
#define VBE_INTNO 0x10
#define INDEXED_MODE 0x105
#define DIRECT_800X600_MODE 0X115
#define START_OF_DISPLAY 0x07

#endif 
