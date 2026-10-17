#include <lcom/lcf.h>
#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_
#define IRQ_KEYBOARD    1
#define IRQ_MOUSE       12
#define BREAK_ESC       0x81
#define UP_ARROW     0x48
#define DOWN_ARROW   0x50
#define LEFT_ARROW   0x4B
#define RIGHT_ARROW  0x4D
#define W_CODE 0x11
#define A_CODE 0x1E
#define S_CODE 0x1F
#define D_CODE 0x20

#define MAKE_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define ENABLE_INT      BIT(0)
#define TIMEOUT_ERROR   BIT(6)
#define PARITY_ERROR    BIT(7)
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER  BIT(1)
#define FULL_MOUSE_BUFFER BIT(5)
#define DISABLE_DATA 0xF5
#define ENABLE_DATA 0xF4

#define KBC_STATUS_REG  0x64
#define KBC_IN_CMD      0x64
#define KBC_OUT_CMD     0x60
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

#define WAIT_KBC        20000
#define MAX_ATTEMPS     10
#endif /* _LCOM_I8042_H_ */
