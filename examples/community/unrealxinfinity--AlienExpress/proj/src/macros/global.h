#ifndef GLOBAL_H
#define GLOBAL_H

#include "struct.h"
//timer 
int counter_timer;
int seconds;
int minutes;


//points
int score;

//info screen

int info_type;

//graphics
vbe_mode_info_t mode_info;
unsigned bytes_per_pixel;
uint16_t x_res;
uint16_t y_res;
unsigned int frame_size;
uint8_t *frame_buffer;
uint8_t *prev_buffer;
uint8_t *map;
uint16_t mode;

//mouse
struct packet mouse_packet;
uint16_t mouse_x;
uint16_t mouse_y;
bool mouse_hover;
int mouse_index;
bool mouse_click;

//xpm images
img_t letters;

img_t a;
img_t b;
img_t c;
img_t d;
img_t e;
img_t f;
img_t g;
img_t h;
img_t i;
img_t j;
img_t k;
img_t l;
img_t m;
img_t n;
img_t o;
img_t p;
img_t q;
img_t r;
img_t s;
img_t t;
img_t u;
img_t v;
img_t w;
img_t x;
img_t y;
img_t z;

img_t two_dots;

img_t numbers[10];
img_t small_numbers[10];


//border positions
int inner_border_up;
int inner_border_left;
int inner_border_right;
int inner_border_down;
int outter_border_right;

//package status
//bool picked=false;

img_t mouse;
simple_animation_t mouse_animation;

img_t player;
AnimatedImg_t animated_img_player;
AnimatedImg_t animated_img_player_bear_hands;
AnimatedImg_t animated_img_player_pistol;
int player_lives;
int max_health;
int count;

img_t enemies_lv1[150];
AnimatedImg_t animated_img_enemy1;

img_t workers[5];
AnimatedImg_t animated_img_workers;

img_t wolf;
AnimatedImg_t animated_img_wolf;
AnimatedImg_t animated_img_friendly_wolf;
int catched;

img_t inventory_box;


img_t packages[6];

img_t teleports[3];

img_t pistols[3];
int pistol_size;

img_t ammo[3];
int ammo_size;

img_t grenades[3];
int grenade_size;

img_t pizza[3];
int food_size;

uint16_t rtc_time[6];
img_t watch;

img_t bullets[50];

//inventory
img_t inventory[9];

//equipment
img_t equipment[3];

uint8_t inventory_size;

img_t selected;

img_t boots;

img_t backpack;

img_t explosion;
img_t explosions[3];
simple_animation_t explosion_effect;

img_t bone;
//uint8_t rtc_time[6];


#endif

