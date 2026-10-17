#ifndef _LCOM_DRAW_H_
#define _LCOM_DRAW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../../devices/graphics/graphics.h"
#include "../../devices/mouse/mouse.h"
#include "../interrupts_actions/interrupts.h"
#include "../context.h"
#include "../allocations_manager/allocations_manager.h"
#include "../../devices/rtc/rtc.h"


///@brief Allocates a double buffer.
///@param mode The mode for frame buffer build.
///@return 0 if successful, 1 otherwise.
int allocate_double_buffer(uint16_t mode);

///@brief Updates the buffers by copying the second frame buffer to the first frame buffer.
void update_buffers();

///@brief Draws a static sprite on the second frame buffer at the specified position.
///@param sprite Pointer to the sprite.
///@return 0 if successful, 1 otherwise.
int draw_movingSprite_xpm(sprite_t *sprite);

///@brief Draws a static sprite on the second frame buffer at the specified position.
///@param sprite Pointer to the sprite.
///@param x The x-coordinate of the sprite's position.
///@param y The y-coordinate of the sprite's position.
///@returns 0 if successful, 1 otherwise.
int draw_staticSprite_xpm(sprite_t *sprite, uint16_t x, uint16_t y);

///@brief Draws the game map by rendering walls, barriers, levers, and traps on the second frame buffer.
void draw_map();

///@brief Draws the real time clock on the screen.
void draw_rtc();

///@brief Draws the countdown timer on the screen.
void draw_timer_countDown();

///@brief @brief Draws the initial menu screen.
void draw_initial_menu_screen();

///@brief Draws the instructions menu screen.
void draw_instructions_menu_screen();

///@brief Draws game screen with the map, the sprites, and the cowntdown.
void draw_game_screen();

///@brief Draws the win screen.
void draw_win_screen();

///@brief Draws the lose screen.
void draw_lose_screen();

///@brief Draws the sprite representing the mouse at its current position.
void draw_newMouse_pos();

///@brief Draws the frame based on the game state and updates the mouse position.
void draw_screen();

///@brief Function pointer type for game screen drawing functions.
typedef void (*funct_t)();

///@brief Array of function pointers for different game screens.
///@note Each function pointer corresponds to a specific screen or state in the game.
static funct_t funcs[] = {
    draw_initial_menu_screen,
    draw_win_screen,
    draw_lose_screen,
    draw_instructions_menu_screen,
    draw_game_screen,
};

#endif
