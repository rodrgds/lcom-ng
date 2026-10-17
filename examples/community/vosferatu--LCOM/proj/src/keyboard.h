#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/** @defgroup keyboard keyboard
 * @{
 *
 * macros for keyboard keys
 */

/**< @brief key break code function */
#define KEY_RELEASE(b) (0x80 | b)

/*! Represents a key */
typedef enum {
    KEY_NONE = 0x0000,
    KEY_ESC = 0x0001,
    KEY_W = 0x0011,
    KEY_I = 0x0017,
    KEY_O = 0x0018,
    KEY_P = 0x0019,
    KEY_ENTER = 0x001C,
    KEY_A = 0x001E,
    KEY_S = 0x001F,
    KEY_D = 0x0020,
    KEY_Z = 0x002C,
    KEY_X = 0x002D,
    KEY_C = 0x002E,
    KEY_1 = 0x0002,
    KEY_2 = 0x0003,
    KEY_3 = 0x0004,
	KEY_SPACE = 0x0039,
    KEY_COMMA = 0x0033,
    KEY_DOT = 0x0034,
    KEY_MINUS = 0x0035,
	KEY_L = 0x0026,
    KEY_E = 0x0012,
    KEY_R = 0x0013,
	KEY_Q = 0x0010,
} KEY;

/** @} */

#endif /* __KEYBOARD_H */
