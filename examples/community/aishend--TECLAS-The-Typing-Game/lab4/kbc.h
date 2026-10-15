#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>

#define OUT_BUFFER 0x60 
#define IN_BUFFER  0x60 
#define ST_REG     0x64 
#define CMD_REG    0x64 

#define KBC_PARITY    BIT(7)
#define KBC_TIMEOUT   BIT(6)
#define KBC_IBF       BIT(1)
#define KBC_OBF       BIT(0)

#define READ_CMD_BYTE  0x20
#define WRITE_CMD_BYTE 0x60
#define KBC_INT_BIT    BIT(0)

#define ESC_BREAK_CODE 0x81
#define DELAY_US       20000

// aproveitado do lab3 adicionamos isto para o lab4 

#define MOUSE_BIT BIT(5)
#define MOUSE_WRITE_BYTE 0xD4    
#define MOUSE_READ_DATA  0xEB    

#define MOUSE_ENABLE_DATA_REP  0xF4
#define MOUSE_DISABLE_DATA_REP 0xF5
#define MOUSE_SET_STREAM_MODE  0xEA
#define MOUSE_SET_REMOTE_MODE  0xF0

#define MOUSE_ACK   0xFA  
#define MOUSE_NACK  0xFE  
#define MOUSE_ERROR 0xFC  

#define MOUSE_LB      BIT(0)
#define MOUSE_RB      BIT(1)
#define MOUSE_MB      BIT(2)
#define MOUSE_BIT3    BIT(3) 
#define MOUSE_X_SIGN  BIT(4)
#define MOUSE_Y_SIGN  BIT(5)
#define MOUSE_X_OVFL  BIT(6)
#define MOUSE_Y_OVFL  BIT(7)

#endif