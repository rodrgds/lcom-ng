#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define DELAY_US 20000

//REGISTERS
#define KBC_ST_REG 0x64 
#define KBC_CMD_REG 0x64

#define KBC_OUT_BUF 0x60 
#define KBC_INP_BUF 0x60 

#define MAKE_CODE BIT(7)

//READ WRITE COMMANDS
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60

//ERRORS
#define TIMEOUT_ERROR BIT(6)
#define PARITY_ERROR BIT(7) 

//HOOK ID
#define KBD_HOOK_ID 1 
#define TIMER0_HOOK_ID 0

#define KBC_IRQ 1


#endif
