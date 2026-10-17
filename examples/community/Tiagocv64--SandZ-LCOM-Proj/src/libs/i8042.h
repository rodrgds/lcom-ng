#pragma once
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define DELAY_US 20000
#define KEYBOARD_IRQ 1

#define OBF 0x1
#define IBF 0x2
#define PAR_ERR 0x80
#define TO_ERR 0x40

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

#define BACKSPACE 0x0E

#define ESC_KEY 0x81

#define PAUSE_KEY 0x19 // P

#define FIRE_KEY 0x39

#define RELOAD_KEY 0x13

#define KEY_1 0x02

#define KEY_2 0x03

#define KEY_3 0x04

#define UP_ARROW_MAKE 0x48
#define UP_ARROW_BREAK 0xC8

#define DOWN_ARROW_MAKE 0x50
#define DOWN_ARROW_BREAK 0xD0

#define LEFT_ARROW_MAKE 0x4B
#define LEFT_ARROW_BREAK 0xCB

#define RIGHT_ARROW_MAKE 0x4D
#define RIGHT_ARROW_BREAK 0xCD


#define CHECK_TWO_BYTE 0xE0
#define CHECK_BREAK_CODE 0x80

#define BIT(n) (0x01<<(n))


#define MOUSE_IRQ 12

#define LB 0x01
#define RB 0x02
#define MB 0x04
#define CHECK_PACK_FIRST 0x08
#define X_SIGN 0x10
#define Y_SIGN 0x20
#define X_OV 0x40
#define Y_OV 0x80

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define MOUSE_COMMAND 0xD4
#define WRITE_COMMAND_BYTE 0x60

#define ENABLE_DATA_REPORTING 0xF4
#define DISABLE_DATA_REPORTING 0xF5
#define SET_REMOTE_MODE 0xF0
#define SET_STREAM_MODE 0xEA
#define MOUSE_READ_DATA 0xEB
