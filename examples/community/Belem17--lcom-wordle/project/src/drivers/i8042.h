#ifndef I8042_H
#define I8042_H
#include <lcom/lcf.h>
#include <lcom/utils.h>
#include "../utils/structs.h"

// arguments must be read and write to port 0x60
#define in_outBuff 0x60
#define statReg 0x64

// commands to write in the status register
#define ReadCommandByte 0x20
#define WriteCommandByte 0x60
#define CheckKBC 0xAA
#define CheckKeyboardInterface 0xAB
#define DisableKBDInterface 0xAD
#define EnableKBDInterface 0xAE
#define DisableMouse 0xA7
#define EnableMouse 0xA8
#define CheckMouseInterface 0xA9
#define WriteByteToMouse 0xD4

//mouse commands
#define Reset 0xFF
#define Resend 0xFE
#define SetDefaults 0xF6
#define Disable 0xF5
#define Enable 0xF4
#define SetSampleRate 0xF3
#define SetRemoteMode 0xF0
#define ReadData 0xEB
#define SetStreamMode 0xEA
#define StatusRequest 0xE9
#define SetResolution 0xE8
#define SetScaling_2_1 0xE7
#define setScaling_1_1 0xE6  

//mouse controller acknowledgment byte
#define Ack_ok 0xFA
#define Nack_invalid 0xFE
#define Error 0xFC

// lcf languages
#define English "EN-US"
#define Portuguese "PT-PT"


#define firstByte 0xE0
#define ESC 0x81
#define SPACE 0x39
#define ENTER 0x1C
#define BACKSPACE 0x0E
#define A 0x1E
#define B 0x30  
#define C 0x2E
#define D 0x20
#define E 0x12
#define F 0x21
#define G 0x22
#define H 0x23
#define I 0x17
#define J 0x24
#define K 0x25
#define L 0x26
#define M 0x32
#define N 0x31
#define O 0x18
#define P 0x19
#define Q 0x10
#define R 0x13
#define S 0x1F
#define T 0x14
#define U 0x16
#define V 0x2F
#define W 0x11
#define X 0x2D
#define Y 0x15
#define Z 0x2C

#define KBC_IRQ 1
#define MC_IRQ 12

int(KBC_subscribe_int)(uint8_t *bit_no);
int(KBC_unsubscribe_int)();
void(KBC_int_handler)(uint8_t *data);
int(readStatusRegister)(uint8_t *read);
int(renable_int)();
int(print)();
int(MC_subscribe_int)(uint8_t *bit_no);
int(MC_unsubscribe_int)();
void(MC_int_handler)(uint8_t *data);
int(readOutBuffRegister)(uint8_t *read);
int(renableMouse_int)();
void(Write_to_struct_packet)(uint8_t bytes[3], struct packet *pp);
int(mouse_disable_data_reporting)(void);
int(mouse_enable_data)(void);
void (updateMouse)(struct Game *game,int16_t dx,int16_t dy);
void atualize(struct Game *game);


#endif /* I8042_H */
