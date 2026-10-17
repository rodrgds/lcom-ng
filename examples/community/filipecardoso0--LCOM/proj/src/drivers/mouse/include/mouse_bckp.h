#ifndef _MOUSE_H_
#define _MOUSE_H_

//Imports stding.h in order to get uint_... variable types
#include <lcom/lcf.h>
#include "../../../utils/include/utils.h"
#include "../../../lib/model/include/sprite.h"
#include "../../../images/mouse_cursor.xpm"

//Global variables
extern int hook_id_mouse; 
extern uint8_t packetbyte; 

typedef struct mouse_cursor{
    int x, y; 
    sprite_t* cursorsprite; 
} mouse_cursor_t; 


int (mouse_subscribe_int)(uint8_t *);
int (mouse_unsubscribe_int)(void); 
bool (mouse_check_IBF)(void); 
int (mouse_write_command)(uint32_t, uint8_t*);
int (mouse_enables_data_reporting)(void); 
int (mouse_disable_data_reporting)(void); 
int (mouse_reset_to_default)(void); 
int (mouse_check_OBF)(void); 
void(mouse_ih)(void); 
void (mouse_parse_packet)(struct packet*);
mouse_cursor_t* cursor_new(void);
void mouse_update(struct packet * pacote);  

//KBC 
#define STATUS_REG 0x64
#define CONTROL_REG 0x64
#define OBF 0x60
#define IBF 0x60

//MOUSE IRQ LINE

#define MOUSE_IRQ 12

//Status Register Information
#define PARITY_ERROR BIT(7) //If 1 -> Error occurred
#define TIMEOUT_ERROR BIT(6) //If 1 -> Error occurred
#define AUX BIT(5) //If 1 -> Data from mouse, if 0 data from keyboard
#define INHIBIT_FLAG BIT(4) //Keyboard is inhibited
#define A2 BIT(3) //Command (1) for KBC or data(0) for device
#define SYS BIT(2) //System flag -> If 0 power in rest, If 1 already initialized
#define IBF_FULL BIT(1) //If 1 -> Don't write commands or arguments
#define OBF_FULL BIT(0) //If 1 -> Information is ready to be read

//Commands to the mouse -> Written into the control register (Port 0x64)
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0X60
#define WRITE_BYTE_TO_MOUSE 0XD4
#define SET_STREAM_MODE 0XEA
#define ENABLE_DATA_REPORTING 0xF4
#define DISABLE_DATA_REPORTING 0XF5

//Byte 1 -> Mouse info

#define Y_OVFL  BIT(7)
#define X_OVFL  BIT(6)
#define Y_SIGN  BIT(5)
#define X_SIGN  BIT(4)
#define ALWAYS_ACTIVE  BIT(3)
#define MID_PRESS  BIT(2)
#define RIGHT_PRESS  BIT(1)
#define LEFT_PRESS  BIT(0)

//Mouse Controller command responses
#define ACK 0xFA   //Acknowledged command or argument
#define NACK 0xFE  //Invalid byte
#define ERROR 0xFC //Second consecutive invalid byte


//Wait for response time 
#define DELAY_US 20000 //IBM's specification of the i8042 -> respond to a command within 20ms

#endif


