#ifndef __MODTECLADO_H
#define __MODTECLADO_H

#define BIT(n) (0x01<<(n))

#define STATUS_PORT 0x64
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define KBD_HOOK_ID  0
#define DELAY_US 20000
#define TWO_BYTE_CODE 0xE0
#define BREAK_CODE_ESC 0x81
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define IRQ1				1
#define PAR_ERR 	BIT(7)
#define TO_ERR 		BIT(6)
#define IBF 		BIT(1)
#define OBF 		BIT(0)
#define RESEND 		0xFE
#define ERROR 		0xFC

unsigned long scan_code;


///@brief scans the buffer to get the scan code
void kbd_int_handler();
///@brief unsubscribes the keyboard interrupts
///@return success
int kbd_unsubscribe_int();
///@brief subscribes the keyboard interrupts
///@return success
int kbd_subscribe_int(void);


#endif
