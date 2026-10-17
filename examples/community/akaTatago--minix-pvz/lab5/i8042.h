#ifndef _I8042_H_
#define _I8042_H_

#include <lcom/lcf.h>

#define KBC_IRQ           1
#define MOUSE_IRQ         12

#define DELAY_US          20000

#define KBC_STATUS_REG    0x64
#define KBC_CMD_REG       0x64
#define KBC_OUT_BUF       0x60

#define KBC_OBF           BIT(0)
#define KBC_IBF           BIT(1)
#define KBC_PAR_ERR       BIT(7)
#define KBC_TO_ERR        BIT(6)
#define KBC_AUX           BIT(5)

#define KBC_READ_CMD      0x20
#define KBC_WRITE_CMD     0x60
#define KBC_INT_KBD       BIT(0)

#define ESC_BREAKCODE     0x81
#define TWO_BYTE_CODE     0xE0
#define MAKECODE_BIT      BIT(7)

#endif /* _I8042_H_ */
