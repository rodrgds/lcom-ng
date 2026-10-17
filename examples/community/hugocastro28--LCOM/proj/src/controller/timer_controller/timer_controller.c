#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/timer/timer.h"
#include "../game_controller.h"
#include "timer_controller.h"
#include "../ghosts_movement_controller/ghosts_movement_controller.h"
#include "../../view/ghosts_view/ghosts_view.h"

unsigned int game_time;
unsigned int timer_minutes;
unsigned int timer_seconds;

unsigned int time_before_pause;

extern unsigned int int_counter;
extern bool game_is_on;

void (update_timer_units)() {
    timer_minutes = game_time / 60;
    timer_seconds = game_time % 60;
}

void (initialize_game_time)() {
    game_time = 120;
    update_timer_units();
}

void (check_time_out)() {
    if (game_time == 0) 
        game_is_on = false;
}

void (increase_time)() {
    game_time += 30;
    update_timer_units();
}

void (decrease_game_time)() {
    game_time--;
    update_timer_units();
    check_time_out();
}

void (timer_interrupt_handler)() {
    timer_int_handler();
    if (int_counter % 60 == 0)
        decrease_game_time();  
    
    if (int_counter % 10 == 0) {
        erase_ghosts();
        move_ghosts();
        draw_ghosts();
    }
}

void (pause_game)() {
    time_before_pause = game_time;
}

void (continue_game)() {
    game_time = time_before_pause;
    update_timer_units();
}
