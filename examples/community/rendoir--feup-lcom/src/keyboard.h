/**
	@file keyboard.h
	@author Daniel Marques
	@author João Almeida

	Prototype of the functions to read keyboard interrupts.
*/



#ifndef KEYBOARD_H
#define KEYBOARD_H

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions responsible for communicating with the keyboard
 */

//TOOLS
#define BIT(n) (0x1<<(n))

//HARDWARE
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define KBD_IRQ 1

//CONVENTIONS
#define TWO_BYTE 0xE0

//MAKE/BREAK CODES
#define MAKE_ENTER 0x1c
#define BREAK_ESC 0x81
#define BREAK_ENTER 0x9c
#define BREAK_UP 0xC8
#define BREAK_DOWN 0xD0


//FUNCTIONS
/**
   @brief Subscribes keyboard interrupts
   @param kbd_id To be ID of the keyboard interrupts
 */
void kbdSubscribe(unsigned int kbd_id);
/**
  @brief Unsubscribes keyboard interrupts
  @param kbd_id ID of the keyboard interrupts
 */
void kbdUnsubscribe(unsigned int kbd_id);
/**
  @brief Reads a byte from the keyboard controller
  @param code To be break/make code read
  @param two_bytes Whether it is a make code with 2 bytes or not
  @return 1 if break code of key , 0 otherwise
 */
 int kbdReadByte(unsigned long int *code, char *two_bytes);


#endif
