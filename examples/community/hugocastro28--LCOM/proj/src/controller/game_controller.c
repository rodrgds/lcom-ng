#include <lcom/lcf.h>

#include <stdint.h>

#include "game_controller.h"
#include "../devices/timer/timer.h"
#include "../devices/kbc/keyboard/keyboard.h"
#include "../devices/kbc/mouse/mouse.h"

#include "model/pacman_model/pacman.h"
#include "model/ghosts_model/ghost.h"
#include "model/coins_model/coin.h"
#include "controller/timer_controller/timer_controller.h"
#include "view/initialize_pixmaps.h"

bool menu_is_on = true;
bool game_is_on = false;
bool pause_is_on = false;
bool rules_is_on = false;
bool exited = false;
bool initializing = true;
unsigned int score = 0;

uint8_t* timer_bit_no;
uint8_t* keyboard_bit_no;
uint8_t* mouse_bit_no;

int (subscribe_game_devices)() {
    timer_bit_no = (uint8_t*) malloc(sizeof(uint8_t));
    if (timer_subscribe_int(timer_bit_no) != 0)
        return 1;
    keyboard_bit_no = (uint8_t*) malloc(sizeof(uint8_t));
    if (keyboard_subscribe_int(keyboard_bit_no) != 0)
        return 1;
    return 0;
}

int (subscribe_menu_devices)() {
    if (enable_data_reporting() != 0)
        return 1;
    mouse_bit_no = (uint8_t*) malloc(sizeof(uint8_t));
    if (mouse_subscribe_int(mouse_bit_no) != 0)
        return 1;
    return 0;    
}

int (unsubscribe_game_devices)() {
    if (timer_unsubscribe_int() != 0)
        return 1;
    if (keyboard_unsubscribe_int() != 0)
        return 1;
    return 0;
}

int (unsubscribe_menu_devices)() {
    if (mouse_unsubscribe_int() != 0)
        return 1;
    if (disable_data_reporting() != 0)
        return 1;
    return 0;
}

void (initialize_game_elements)() {
    set_pacman_position();
    set_ghosts_positions();
    set_small_coins_positions();
    set_big_coins_position();

    initialize_all_pixmaps();
    
    initialize_game_time(); 
}
