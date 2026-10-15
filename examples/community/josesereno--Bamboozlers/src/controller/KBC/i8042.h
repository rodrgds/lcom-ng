#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_


/* IRQ lines */
#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

/* Scancodes */
#define ESC_BREAK 0x81
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT_PRESS 0x4B
#define ARROW_LEFT_RELEASE 0xCB
#define ARROW_RIGHT_PRESS 0x4D
#define ARROW_RIGHT_RELEASE 0xCD
#define ENTER 0x1C
#define R_KEY 0x13
#define E_KEY 0x12
#define Q_KEY 0x10

/* KBC register ports */
#define KBC_OUT_BUF 0x60
#define KBC_CMD_REG 0x60
#define KBC_IN_BUF 0x64
#define KBC_ST_REG 0x64

/* KBC status register */
#define KBC_PARITY_ERR BIT(7)
#define KBC_TIMEOUT_ERR BIT(6)
#define KBC_AUX BIT(5)
#define KBC_IBF BIT(1)
#define KBC_OBF BIT(0)

/* KBC delay*/
#define WAIT_KBC tickdelay(micros_to_ticks(20000))

/* KBC commands */
#define KBC_READ_CMD  0x20
#define KBC_WRITE_CMD  0x60

/* Mouse commands */
#define ENABLE_DATA_REPORTING 0xF4
#define DISABLE_DATA_REPORTING 0xF5
#define WRITE_BYTE_TO_MOUSE 0xD4

/* Mouse acknowledge byte */
#define MOUSE_ACK 0xFA
#define MOUSE_NACK 0xFE
#define MOUSE_ERROR 0xFC

/* KBC command byte */
#define KBC_INT  BIT(0)
#define KBC_INT2 BIT(1)
#define KBC_DIS  BIT(4)
#define KBC_DIS2 BIT(5)

#endif /* _LCOM_I8042_H */
