#ifndef I8042_H
#define I8042_H

#define KBC_BRK_ESC_KEY 0x81 
#define KBC_MSB_SCNCD BIT(7)
#define KEYBOARD_IRQ 1 
#define MOUSE_IRQ 12 

#define KBC_STAT_REG 0x64 
#define KBC_IN_BUF_CMD 0x64 
#define KBC_IN_BUF_ARG 0x60 
#define KBC_OUT_BUF 0x60 

#define KBC_PARITY BIT(7) 
#define KBC_TIMEOUT BIT(6) 
#define KBC_AUX BIT(5) 
#define KBC_INH BIT(4) 
#define KBC_A2 BIT(3) 
#define KBC_SYS BIT(2) 
#define KBC_IBF BIT(1) 
#define KBC_OBF BIT(0) 

#define KBC_R_CMD_B 0x20 
#define KBC_W_CMD_B 0x60 
#define KBC_CHK 0xAA 
#define KBC_CHK_KBD_I 0xAB 
#define KBC_DIS_KBD_I 0XAD 
#define KBC_EN_KBD_I 0XAE 

#define KBC_DIS2 BIT(5) 
#define KBC_DIS BIT(4) 
#define KBC_INT2 BIT(1) 
#define KBC_INT BIT(0)

#define KBC_2BYTE_CODE 0xE0 

#define KBC_ACK 0xFA 
#define KBC_NACK 0xFE 
#define KBC_ERROR 0xFC 
#define MOUSE_EN_DATA_REP 0xF4
#define MOUSE_DIS_DATA_REP 0xF5
#define MOUSE_STREAM_MODE 0xEA
#define MOUSE_REMOTE_MODE 0xF0
#define MOUSE_READ_DATA 0xEB
#define MOUSE_WRITE_BYTE 0xD4

#define MOUSE_LB            BIT(0)
#define MOUSE_RB            BIT(1)
#define MOUSE_MB            BIT(2)
#define MOUSE_X_SIGNAL      BIT(4)
#define MOUSE_Y_SIGNAL      BIT(5)
#define MOUSE_X_OVERFLOW    BIT(6)
#define MOUSE_Y_OVERFLOW    BIT(7)

#endif /* I8042_H */
