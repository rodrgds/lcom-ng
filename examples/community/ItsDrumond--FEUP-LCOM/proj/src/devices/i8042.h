#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US 20000
#define KBC_ESC 0x81
#define KBC_CMD_REG 0x64

#define KBD_PAR_ERR BIT(7)
#define KBD_TO_ERR BIT(6)
#define KBD_ST_IBF BIT(1)
#define OUT_BFF_FULL BIT(0)
#define KBC_AUX_MOUSE BIT(5)

#define KBD_ESC 0x81

#define KBC_ACK 0xFA
#define MOUSE_DIS_DATA_REP 0xF5
#define MOUSE_EN_DATA_REP 0xF4
#define PACKET_RIGHT_BOTTON BIT(1)
#define PACKET_LEFT_BOTTON BIT(0)
#define PACKET_MIDDLE_BOTTON BIT(2)
#define PACKET_X_NEGATIVE BIT(4)
#define PACKET_Y_NEGATIVE BIT(5)
#define PACKET_X_OVERFLOW BIT(6)
#define PACKET_Y_OVERFLOW BIT(7)

#define KBD_REENABLE_INT BIT(0)



#define KBC_IRQ 1
#define MOUSE_IRQ 12
#define KBD_IRQ 1

#define KBC_ST_IBF BIT(1)
#define KBC_OUT_BUF 0x60

#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR BIT(6)


#define TWO_BYTE 0xE0
#define MAKE 0x80

#define OUT_BUF 0x60
#define IN_BUF 0x64
#define IN_BUF_ARGS 0x60
#define STAT_REG 0x64

#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60
#define KBC_REENABLE_INT BIT(0)

#endif
