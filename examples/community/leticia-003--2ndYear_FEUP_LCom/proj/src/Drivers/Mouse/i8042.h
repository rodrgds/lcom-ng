#ifndef I8042_H
#define I8042_H

#define MOUSE_IRQ 12

#define STAT_REG    0x64
#define KBC_CMD_REG 0x64
#define OUT_BUF     0x60
#define IN_BUF      0x60

#define OBF     BIT(0)
#define IBF     BIT(1)
#define AUX     BIT(5)
#define PAR_ERR BIT(7)
#define TO_ERR  BIT(6)

#define WRITE_BYTE_TO_MOUSE 0xD4

#define MOUSE_ENABLE_DR   0xF4
#define MOUSE_DISABLE_DR  0xF5

#define ACK   0xFA
#define NACK  0xFE
#define ERROR 0xFC

#define ATTEMPTS 5
#define DELAY_US 20000

#endif 
