#ifndef _LCOM_I8042_KEYBOARD_H_
#define _LCOM_I8042_KEYBOARD_H_

#define KEYBOARD_IRQ 1

#define BREAK_ESC 0x81
#define MAKE_COKE BIT(7)
#define TWO_BYTES 0xE0
#define ENABLE_INTERRUPTS BIT(0)

#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define OUTPOUT_BUF_FULL BIT(0)
#define INPUT_BUF_FULL BIT(1)

#define KBC_STATUS_REG 0x64
#define KBC_IN_CMD 0x64
#define KBC_OUT_CMD 0x60
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60

#define WAIT_KBC 20000
#define MAX_ATTEMPTS 10

#endif
