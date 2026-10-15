#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/* Mouse commands*/

#define WRITE_BYTE_MOUSE  0xD4  // Comando para avisar o KBC que vamos escrever no rato
#define ACK               0xFA  // Acknowledged (Tudo OK)
#define NACK              0xFE  // Not Acknowledged (Erro, tentar de novo)
#define ERROR             0xFC
#define MOUSE_IRQ         12
#define EN_DATA_REPORT    0xF4  // Enable Data Reporting (Ativa o envio de pacotes)
#define DIS_DATA_REPORT   0xF5  // Disable Data Reporting (Desativa o envio de pacotes)
#define SET_STREAM_MODE   0xEA  // Set Stream Mode (Modo contínuo, por defeito)

#define MOUSE_LB         BIT(0) // Left Button
#define MOUSE_RB         BIT(1) // Right Button
#define MOUSE_MB         BIT(2) // Middle Button
#define MOUSE_SYNC_BIT   BIT(3) // Bit 3 do 1º byte (Sempre a 1, usado para sincronização)
#define MOUSE_X_SIGN     BIT(4) // Sinal do X (1 se negativo)
#define MOUSE_Y_SIGN     BIT(5) // Sinal do Y (1 se negativo)
#define MOUSE_X_OVFL     BIT(6) // X Overflow
#define MOUSE_Y_OVFL     BIT(7) // Y Overflow

/* -------- */

#define KBD_IRQ 1
#define DELAY_US 20000

/* Portos */
#define MOUSE_OUT_BUF   0x60
#define MOUSE_IN_BUF    0x60
#define MOUSE_STAT_REG  0x64
#define MOUSE_CMD_REG   0x64

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

