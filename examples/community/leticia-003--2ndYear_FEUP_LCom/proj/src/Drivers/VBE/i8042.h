#ifndef I8042_H
#define I8042_H

#include <lcom/lcf.h>

#define STAT_REG  0x64
#define KBC_CMD_REG 0x64
#define IN_BUF    0x60
#define OUT_BUF   0x60
#define IRQ_KBD 1

#define PAR_ERR   BIT(7)
#define TO_ERR    BIT(6)
#define OBF       BIT(0)
#define IBF       BIT(1)

#define DELAY_US 20000

#define KBC_STAT_REG  0x64
#define KBC_OUT_BUF   0x60
#define KBC_CMD_REG   0x64
#define KBC_ARG_REG   0x60

#define KBC_READ_CMD_BYTE  0x20
#define KBC_WRITE_CMD_BYTE 0x60

#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR  BIT(6)
#define KBC_OBF     BIT(0)

#define KBC_IBF BIT(1)

#define IRQ_MOUSE 12

#define OUT_BUF 0x60
#define KBC_CMD_REG 0x64
#define WRITE_BYTE_TO_MOUSE 0xD4

#define ENABLE_DATA_REPORTING 0xF4
#define DISABLE_DATA_REPORTING 0xF5

#define ACK 0xFA

#define OUT_BUF 0x60


#endif 
