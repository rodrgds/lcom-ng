#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t data[2];
    uint8_t byte;
    bool complete;
} keyboard_packet_t;

typedef enum {
    KEY_ENTER = 0x1C,
    KEY_ESC = 0x01,
    KEY_SPACE = 0x39,
    KEY_BACKSPACE = 0x0E,
    KEY_UP = 0x48,
    KEY_DOWN = 0x50,
    KEY_LEFT = 0x4B,
    KEY_RIGHT = 0x4D,
    KEY_LSHIFT = 0x2A,
    KEY_RSHIFT = 0x36,
    KEY_LCTRL = 0x1D,
    KEY_RCTRL = 0x9D,
    KEY_A = 0x1E,
    KEY_W = 0x11,
    KEY_D = 0x20,
    KEY_S = 0x1F,
    KEY_Q = 0x10,
    KEY_E = 0x12,
    KEY_NONE = 0x00,
    KEY_LEFT_BREAK = 0xCB,
    KEY_RIGHT_BREAK = 0xCD,
    KEY_A_BREAK = 0x9E,
    KEY_D_BREAK = 0xA0,
} keyboard_key;

/**
 * @brief Subscribes to keyboard 
 *
 * @param bit_no Pointer that will be pointed to the keyboard hook_id.
 * @return int 1 if an error occurs, 0 otherwise
 */
int keyboard_subscribe_int(uint8_t* bit_no);

/**
 * @brief Unsubscribes from keyboard
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int keyboard_unsubscribe_int();

/**
 * @brief Reads a byte received from the keyboard and updates the packet accordingly.
 *
 * @param b The byte received from the keyboard.
 */
void k_read_byte(uint8_t b);

/**
 * @brief Keyboard interrupt handler. Reads data from the keyboard and updates the packet.
 */
void keyboard_ih();

/**
 * @brief Restores the keyboard interrupt handler to its original state.
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int keyboard_restore_int();

/**
 * @brief Retrieves the key from the keyboard packet.
 *
 * @return Returns the key from the keyboard packet. If the packet is not complete, KEY_NONE is returned.
 */
keyboard_key keyboard_get_key();

#endif /* __KEYBOARD_H */
