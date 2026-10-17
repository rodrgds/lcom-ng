/*
 * keyboard.h
 *
 *  Created on: Oct 20, 2014
 *      Author: daniel
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "i8254.h"

//#define OK 0
#define KBD_IRQ 1
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define BIT_SIG_0 0x7F
#define LED 0xED
#define LEDS_COMM 0xED
#define RESEND 0xFE
#define LED_ERROR 0xFC
#define ESC_KEY 0x81
#define RETURN_KEY 0x9C
#define BACKSPACE 0x8E
#define ACK 0xFA
#define TWO_BYTES 0xe0
#define PAR_ERR 0x80
#define TO_ERR 0x40
#define ESC_BREAK_CODE 0x81
#define ENTER_KEY 0x009C
#define W_KEY 0x0091
#define S_KEY 0x009F
#define B_KEY 0x00B0

// TECLAS

#define KEY_A 0x9E
#define KEY_B 0xB0
#define KEY_C 0xAE
#define KEY_D 0xA0
#define KEY_E 0x92
#define KEY_F 0xA1
#define KEY_G 0xA2
#define KEY_H 0xA3
#define KEY_I 0x97
#define KEY_J 0xA4
#define KEY_K 0xA5
#define KEY_L 0xA6
#define KEY_M 0xB2
#define KEY_N 0xB1
#define KEY_O 0x98
#define KEY_P 0x99
#define KEY_Q 0x90
#define KEY_R 0x93
#define KEY_S 0x9F
#define KEY_T 0x94
#define KEY_U 0x96
#define KEY_V 0xAF
#define KEY_W 0x91
#define KEY_X 0xAD
#define KEY_Y 0x95
#define KEY_Z 0xAC
#define KEY_0 0x8B
#define KEY_1 0x82
#define KEY_2 0x83
#define KEY_3 0x84
#define KEY_4 0x85
#define KEY_5 0x86
#define KEY_6 0x87
#define KEY_7 0x88
#define KEY_8 0x89
#define KEY_9 0x8A
#define KEY_SPACE 0x00B9







//extern unsigned long keyboard;


#define DELAY_US 20000

/**
 * @brief subscribe keyboard interrupts
 *
 * @return -1 if failed otherwise the bit of the hook id
 */
int KBD_subscribe_int(void );

/**
 * @brief unsubscribe keyboard interrupts
 *
 * @return -1 if failed
 */
int KBD_unsubscribe_int();

/**
 * @brief gets the keyboard code
 *
 * @return the code of the keyboard
 */
int KBD_handler_C();

/**
 * @brief returns the char that corresponds to the keyboard code
 *
 * @param keyboard code
 *
 * @return char correspondent
 */
char getLetra(unsigned long keyboard); // de acordo com os keyboard input retorna a letra char correspondente

/**
 * @brief gets the position of a letter in the alphabet bitmap
 *
 * @param letter pretended
 *
 * @return x coord in the bitmap
 */
int getLetterPos(char letter); // retorna a posicao da letra no bitmap

/**
 * @brief sleep for a second
 */
int wait_a_second();

#endif /* KEYBOARD_H_ */
