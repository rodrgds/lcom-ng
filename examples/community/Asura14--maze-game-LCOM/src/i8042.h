#ifndef __I8042_H
#define __I8042_H


#define BIT(n) (0x01<<n)

#define TIMER0_IRQ	    0    /**< @brief Timer 0 IRQ line */
#define KEYBOARD_IRQ 	1
#define RTC_IRQ 8

#define STAT_REG 0x64	// Status Register for reading the KBC state
#define OUT_BUF 0x60
#define IN_BUF 0x60


// C@KBD responds with one of 3 values:
#define ACK 0xFA
#define ERROR 0xFC	// entire sequence should be restarted
#define RESEND 0xFE	// latest byte should be written again


#define ESC_BREAK_CODE 0x81	// ESC break code
#define ESC_MAKE_CODE 0x01 // ESC make code

#define DOWN_BREAK_CODE 0xD0
#define DOWN_MAKE_CODE 0xD0-0x80
#define LEFT_BREAK_CODE 0xCB
#define LEFT_MAKE_CODE 0xCB-0x80
#define RIGHT_BREAK_CODE 0xCD
#define RIGHT_MAKE_CODE 0xCD-0x80
#define UP_BREAK_CODE 0xC8
#define UP_MAKE_CODE 0xC8-0x80


/*
 * set keyboard LEDs
 */

#define SET_LED 0xED	// keyboard command to set keyboard leds
#define IBF_FLAG BIT(1) // 1 if IBF is set -> Input Buffer Full

#define OBF BIT(0)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)

#endif
