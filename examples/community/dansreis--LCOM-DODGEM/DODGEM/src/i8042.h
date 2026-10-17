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

//#include "timer.h"


#define KBD_IRQ 1
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define BIT_SIG_0 0x7F
#define LED 0xED
#define ESC_KEY 0x81
#define LEDS_COMM 0xED
#define RESEND 0xFE
#define LED_ERROR 0xFC
#define ACK 0xFA
#define TWO_BYTES 0xe0
#define PAR_ERR 0x80
#define TO_ERR 0x40

#define KBDCOMMAND 0xD4
#define MOUSE_IRQ 12
#define STREAMMODE 0xEA
#define STATUSREQUEST 0xE9
#define ENABLE_MOUSE 0xA8
#define SEND_PACKET 0xF4
#define DISABLE_STREAM 0xF5



#define DELAY_US 20000
void print_config(unsigned char *packets);


#endif /* KEYBOARD_H_ */
