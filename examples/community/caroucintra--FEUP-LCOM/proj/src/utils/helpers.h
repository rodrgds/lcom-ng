#ifndef _LCOM_HELPERS_H_
#define _LCOM_HELPERS_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "game.h"
#include "collision_handler.h"

Sprite get_horizontal_arrow();
Sprite get_background();
Sprite get_background_esc();
Sprite get_button_food();
Sprite get_start_button();
Sprite get_exit_button();
Sprite get_button_bath();
Sprite get_button_fun();
Sprite get_button_love();
Sprite get_button_sleep();
Sprite get_arrow();
Sprite get_hour_unit();
Sprite get_hour_ten();
Sprite get_min_unit();
Sprite get_min_ten();
Sprite get_dois_pontos();
Sprite get_mouse();
Sprite get_violetchi();


void get_key(uint8_t scancode[], int size);

void needs_button_pressed(onButton game_onButton);

bool verify_timer_events();

void reduce_hunger_scale();

void reset_hunger_counter();

void reduce_to_sleep_scale();

void reset_to_sleep_counter();

void reduce_hygiene_scale();

void reset_hygiene_counter();

void reduce_love_scale();

void reset_love_counter();

void reduce_play_scale();

void reset_play_counter();

int (start_scene)(Game game);

int (draw_bg)();

int (draw_buttons)();

int (draw_mouse)();

int (draw_game_over)();

void (update_mouse)();

void (update_arrow)();

void update_buttons(Needs violet_needs);

int (do_animation)();

int love_animation();

void (reset_timer_counters)();

int (draw_menu)();

void (initialize_scene)();

void (set_clock_min_unit)(uint8_t min);

void (set_clock_min_ten)(uint8_t min);

void (set_clock_hour_unit)(uint8_t hour);

void(set_clock_hour_ten)(uint8_t hour);

void (update_clock)();

int (draw_clock)();

int clear_clock();

void (mouse_handle_pos)();

void (check_action)();

void (reset_petting_counter)();

bool check_end_love();

int change_animation();

void reset_violetchi();


#endif
