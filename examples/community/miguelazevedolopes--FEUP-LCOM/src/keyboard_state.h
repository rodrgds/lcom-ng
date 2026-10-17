#include <stdint.h>
#include <stdio.h>

/** @defgroup keyboard_state keyboard_state.h
 * @{
 *
 * This file contains functions that handle the state of the keyboard.
 * 
 */

typedef enum  {
    up_pressing,
    up_pressed,
    down_pressing,
    down_pressed,
    escape,
    right_pressing, 
    right_pressed,
    first_byte,

    up_right_pressed,
    down_right_pressed,

    up_right_pressing,
    down_right_pressing, 
    
    up_pressed_right_pressing,
    down_pressed_right_pressing, 
    
    up_pressing_right_pressed,
    down_pressing_right_pressed,

    w_pressing,
    w_pressed,

    s_pressing,
    s_pressed,

    null,
}keyboard_state;


void(print_state)(keyboard_state * state);

void (update_state)(keyboard_state * state , uint8_t scancode,bool is_left);

void keyboard_move_bar(keyboard_state state, bool is_left);
/**@}*/
