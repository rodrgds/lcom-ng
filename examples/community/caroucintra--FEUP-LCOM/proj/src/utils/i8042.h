#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

#define FIRST_BYTE_SCAN 0xE0

#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60

#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define AUX BIT(5)
#define OBF  BIT(0)
#define IBF BIT(1)

#define READ_COM_BYTE 0x20
#define WRITE_COM_BYTE 0x60
#define CHECK_KBC 0xAA
#define CHECK_KBD_INTERFACE 0xAB
#define DIS_KBD_INTERFACE 0xAD
#define EN_KBD_INTERFACE 0xAE

#define POLL_DELAY 20000

#define BREAK_CODE BIT(7)

#define ENABLE_INT_KBD BIT(0)
#define ENABLE_INT_MOUSE BIT(1)
#define DISABLE_MOUSE BIT(5)
#define DISABLE_KBD_INTERFACE BIT(4)


#define VBE_MODE 0x14C


#define ESC 0x81
#define ARROW_LEFT 0x4b
#define ARROW_RIGHT 0x4d
#define ARROW_DOWN 0xd0
#define ARROW_UP  0xc8
#define SPACE 0x39
#define INVALID 0x00

#define HUNGER_TIME 60*10 //remember: 60*60 == 1 min para o timer 
#define SLEEP_TIME 60*60 //remember: 60*60 == 1 min para o timer 
#define BATH_TIME 60*30 //remember: 60*60 == 1 min para o timer 
#define LOVE_TIME 60*60 //remember: 60*60 == 1 min para o timer
#define PLAY_TIME 60*20 //remember: 60*60 == 1 min para o timer
#define DELAY 500000
#define PETTING_TIME 60*5
#define ANIMATION_TIME 60

#define VIOLETCHI_HEIGHT 303
#define VIOLETCHI_WIDTH 320 

#define MIN_LOVE_MOVEM 50
//MOUSE PACKET

#define LEFT BIT(0)
#define RIGHT BIT(1)
#define MIDDLE BIT(2)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVF BIT(6)
#define Y_OVF BIT(7)


//KBC COMMANDS

#define WRITE_CB 0x60


#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC
#define DISABLE_DATA_REPORTING 0xF5 
#define ENABLE_DATA_REPORTING 0xF4 

#define BYTE_TO_MOUSE 0xD4


#define MIN_SLOPE 1.0

typedef enum { INIT, DRAW_UP, TRANSITION, DRAW_DOWN,
DETECTED } state_t;

typedef enum { LB, MB, RB, NOT_PRESSED} pressed_t;




#endif /* _LCOM_I8042_H */
