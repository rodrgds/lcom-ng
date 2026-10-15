#ifndef I8042_H
#define I8042_H

#define KBC_IRQ 1

#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60

#define KBC_STAT_REG 0x64
#define KBC_CMD_REG 0x64

#define PARITY_ERR BIT(7)
#define TIMEOUT_ERR BIT(6)
#define MOUSE_DATA BIT(5)

#define ESC_BREAK_CODE 0x81

#define IBF BIT(1) //input buffer full
#define OBF BIT(0) //output buffer full
#define KBC_MAX_ATTEMPTS 10
#define KBC_DELAY_US 20000

#endif //I8042_H
