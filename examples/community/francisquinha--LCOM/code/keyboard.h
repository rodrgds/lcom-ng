#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/** @file keyboard.h
 * @brief file containing declarations of the functions that deal with the keyboard
 */

/** @defgroup keyboard keyboard
 * @{
 *
 * @brief functions for using the i8042 keyboard
 */

/**@{
 * @brief Keyboard info
 */
typedef struct {
	long scan_code;				/**< @brief last keyboard scan code */
	long make_code;				/**< @brief last keyboard make code */
	long break_code;			/**< @brief last keyboard break code */
	char last_make;				/**< @brief indicator if the last code was a make code */
	char dual_code;				/**< @brief indicator if the last code was a dual code */
} Keyboard;
/** @}*/


/**
 * @brief creates a new keyboard structure
 * @return the keyboard created
 */
Keyboard *create_keyboard();

/**
 * @brief to destroy a previously created keyboard
 * @param keyboard we want to destroy
 */
void destroy_keyboard(Keyboard *keyboard);

/**
 * @brief to handle keyboard interruptions
 * @param keyboard whose interruption we want to handle
 */
void keyboard_int_handler(Keyboard *keyboard);

/** @} end of keyboard */

#endif /* __KEYBOARD_H */
