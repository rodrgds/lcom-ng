#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BIT(n) (0x01<<(n))

#define KBD_IRQ         1
#define MOUSE_IRQ       12


// Registers
#define STAT_REG        0x64
#define KBC_CMD_REG     0x64
#define IN_BUF          0x60
#define OUT_BUF         0x60


// KBC Commands (command byte related)
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

// Status bits
#define OBF             BIT(0)
#define IBF             BIT(1)
#define AUX             BIT(5)
#define TO_ERR          BIT(6)
#define PAR_ERR         BIT(7)



#define DIS2            BIT(5)
#define DIS             BIT(4)
#define INT2            BIT(1)
#define INT             BIT(0)

#define WAIT_KBC 		20000


// Keycodes
#define BYTE2           0xe0

#define ESC             0x81

// KBC Write to Mouse Command

#define MOUSE_WRITE_CMD 0xD4

// ------- Mouse Commands
#define EN_DATA_REPORT  0xF4
#define DIS_DATA_REPORT 0xF5
#define RESET           0xFF
#define SET_STREAM      0xEA
#define SET_REMOTE      0xF0
#define READ_DATA       0xEB

// ------- Responses
#define ACK             0xFA
#define NACK            0xFE
#define ERROR           0xFC

// Mouse data parsing

#define LB              BIT(0)
#define RB              BIT(1)
#define MB              BIT(2)
#define X_SIGN          BIT(4)
#define Y_SIGN          BIT(5)
#define X_OVF           BIT(6)
#define Y_OVF           BIT(7)


#endif /* _LCOM_I8042_H_ */
