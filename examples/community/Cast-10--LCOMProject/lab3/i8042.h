#pragma once
#include <lcom/lcf.h>


#define ESC 0x01

#define KBD_IRQ 1

#define BREAKCODE BIT(7)

#define CODE_PREFIX 0xE0

#define OUTPUT_BUFFER 0x60

#define INPUT_BUFFER 0x60

#define STATUS_REGISTER 0x64

#define COMMAND_REGISTER 0x64

// Status Register bit-mask

#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define AUX BIT(5)
#define INPUT_BUFFER_FULL BIT(1)
#define OUTPUT_BUFFER_FULL BIT(0)

// KBC commands

#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60
#define CHECK_KBC 0xAA //returns 0x55 if ok 0xFC if error
#define CHECK_KEYBOARD_INTERFACE 0xAB
#define DISABLE_KBD 0xAD
#define ENABLE_KBD 0xAE

// command byte

#define DISABLE_MOUSE_INTERFACE BIT(5)
#define DISABLE_KEYBOARD_INTERFACE BIT(4)
#define ENABLE_MOUSE_INTERRUPTS BIT(1)
#define ENABLE_KEYBOARD_INTERRUPTS BIT(0)


// ==================================LAB4=====================================

#define WRITE_TO_MOUSE 0xD4
#define ACK 0xFA
#define MOUSE_ERROR  0xFC
#define DISABLE_DATA_REPORTING 0xF5
#define ENABLE_DATA_REPORTING  0xF4
#define SET_STREAM_MODE        0xEA
#define SYNCRONIZED BIT(3)

#define MOUSE_IRQ 12


#define LEFT_BUTTON    BIT(0)
#define RIGHT_BUTTON   BIT(1)
#define MIDDLE_BUTTON  BIT(2)
#define X_SIGN         BIT(4)
#define Y_SIGN         BIT(5)
#define X_OVERFLOW     BIT(6)
#define Y_OVERFLOW     BIT(7)
