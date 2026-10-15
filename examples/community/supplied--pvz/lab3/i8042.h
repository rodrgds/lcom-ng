#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KBD_IRQ 1
#define DELAY_US 20000

/* Portos do KBC */
#define KBC_OUT_BUF   0x60
#define KBC_IN_BUF    0x60
#define KBC_STAT_REG  0x64
#define KBC_CMD_REG   0x64

/* Bits do Status Register */
#define KBC_OBF       BIT(0)
#define KBC_IBF       BIT(1)
#define KBC_AUX       BIT(5)
#define KBC_TIMEOUT   BIT(6)
#define KBC_PARITY    BIT(7)

/* Scancodes Especiais */
#define ESC_BREAKCODE 0x81
#define TWO_BYTE_CODE 0xE0

/* Comandos do KBC */
#define KBC_READ_CMD  0x20
#define KBC_WRITE_CMD 0x60
#define KBC_INT_EN    BIT(0) // Bit para ativar interrupções do teclado no Command Byte

#endif /* _LCOM_I8042_H_ */

