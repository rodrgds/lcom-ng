#ifndef MOUSE_H
#define MOUSE_H


//i8042

#include <minix/syslib.h>
#include <minix/drivers.h>
#define STAT_REG 		0x64
#define KBC_CMD_REG 	0x64
#define OUT_BUF 		0x60
#define DATA_REG 		0x60



//1ºbyte
#define LEFTB 0
#define RIGHTB 1
#define MIDDLEB 2
#define SYNCB 3
#define XSIGN 4
#define YSIGN 5
#define XOVER 6
#define YOVER 7
//2ºbyte X
//3ºbyte y
#define DTRPRT 0xF4 //enable data reporting
#define STREAM 0xEA //Set Stream Mode
#define Status 0xE9 //request status

//status 1ºbyte
//0-left button
//1 - right button
//2- middle button
//3- always 0
#define SCALL 4
#define DTREP 5 //1 if data report enabled
#define MODE 6 // 1- remote mode, 0-stream mode

int subscribe_mouse();
int unsubscribe_mouse();
void clear_buffer();
void mouse_int(Campo_t field[][getLado()], int me,int multi);
int getMousex();
int getMousey();
void setMousex();
void setMousey();
int getLost();
void setLost(int n);
int getli();
void setli(int n);
int getlj();
void setlj(int n);
int getFirstClick();
void setFirstClick(int n);
void clean_mouse();



#endif


