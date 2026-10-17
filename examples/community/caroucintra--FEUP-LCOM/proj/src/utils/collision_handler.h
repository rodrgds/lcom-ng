#include "game.h"
#include "helpers.h"

int (clear_sprite)(Sprite* sp, Sprite* collision_element);
int (clear_sprite_up)(Sprite* sp, Sprite* collision_element, int bg_delta_y, int element_delta_y);
int (clear_sprite_down)(Sprite* sp, Sprite* collision_element, int bg_delta_y, int element_delta_y);
int (clear_sprite_left)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x);
int (clear_sprite_right)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x);
int (clear_sprite_top_right)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x, int bg_delta_y, int element_delta_y);
int (clear_sprite_bottom_left)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x, int bg_delta_y, int element_delta_y);
int (clear_sprite_top_left)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x, int bg_delta_y, int element_delta_y);
int (clear_sprite_bottom_right)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x, int bg_delta_y, int element_delta_y);
bool (check_collision_up)(Mouse mouse, Sprite element);
bool (check_collision_down)(Mouse mouse, Sprite element);
bool (check_collision_left)(Mouse mouse, Sprite element);
bool (check_collision_right)(Mouse mouse, Sprite element);
bool (check_collision_inside)(Mouse mouse, Sprite element);
bool (check_collision_top_left)(Mouse mouse, Sprite element);
bool (check_collision_top_right)(Mouse mouse, Sprite element);
bool (check_collision_bottom_left)(Mouse mouse, Sprite element);
bool (check_collision_bottom_right)(Mouse mouse, Sprite element);
bool check_collision_background(Mouse mouse);
bool check_collision_button_food(Mouse mouse);
bool check_collision_button_bath(Mouse mouse);
bool check_collision_button_fun(Mouse mouse);
bool check_collision_button_love(Mouse mouse);
bool check_collision_button_sleep(Mouse mouse);
bool check_collision_arrow(Mouse mouse);
bool check_collision_hour_unit(Mouse mouse);
bool check_collision_hour_ten(Mouse mouse);
bool check_collision_min_unit(Mouse mouse);
bool check_collision_min_ten(Mouse mouse);
bool check_collision_dois_pontos(Mouse mouse);
bool check_collision_violetchi(Mouse mouse);
void check_any_collision(Mouse mouse);


