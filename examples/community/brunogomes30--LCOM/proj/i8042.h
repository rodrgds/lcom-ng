#ifndef I8042_H
#define I8042_H

#define KBD_IRQ 		1 
#define MOUSE_IRQ     12

//Status register byte
#define OUT_BUF_STATUS 	BIT(0) 
#define TIME_OUT_ERROR 	BIT(6) 
#define PARITY_ERROR 	BIT(7) 

#define STATUS_REG 	0x64   
#define KBD_CMD_BUF 0x64 



//Keycodes -- Make code, break code has BIT 7 set to 1
#define KEYCODE_W 0x11
#define KEYCODE_A 0x1e
#define KEYCODE_S 0x1f
#define KEYCODE_D 0x20
#define KEYCODE_ENTER 0x1c
#define KEYCODE_ESC 0x01
#define KEYCODE_1 0x02
#define KEYCODE_2 0x03
#define KEYCODE_3 0x04


#define BREAK_CODE(code) ( code | BIT(7) )
#endif //I8042_H
