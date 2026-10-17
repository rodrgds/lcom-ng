#ifndef KBC_H
#define KBC_H

#include <lcom/lcf.h>
#include "i8042.h"
#include "utils.h"

#define KBD_KEY 30
#define IRQ_SET_KBD BIT(KBD_KEY)

#define ESC 0
#define TAB 1         // 0x0f
#define BACKSPACE 2   // 0e
#define SHIFT 3       // 2a
#define CTRL 4        // 1d
#define ALT 5         // 38
#define ENTER 6       // 1c
#define ARROW_UP 7    // e048
#define ARROW_LEFT 8  // e04b
#define ARROW_RIGHT 9 // e04d
#define ARROW_DOWN 10 // e050

/**
 * @brief Subscribes and enables keyboard interrupts
 * 
 * @return int 0 if successful, 1 otherwise
 */
int keyboard_subscribe_interrupt();

/**
 * @brief Unsubscribes keyboard interrupts
 * 
 * @return int 0 if successful, 1 otherwise
 */
int keyboard_unsubscribe_interrupt();

/**
 * @brief Reads the scancode from the output buffer
 * 
 * @param ret Pointer to the variable where the scancode will be stored
 */
void read_scancode(u8_t* ret);

/**
 * @brief converts a char to a scancode
 * @param c char to be converted
 * @return u8_t Scancode read
 */
u8_t char_to_scancode(char c);


#endif
