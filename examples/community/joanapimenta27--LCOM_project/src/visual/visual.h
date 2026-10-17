/**
 * @file visual.h
 * @brief Header file containing declarations related to visual rendering.
 */

#ifndef _VISUAL_H_
#define _VISUAL_H_

#include <lcom/lcf.h>
#include "../controller/video/graphics.h"
#include "../controller/kbc/mouse.h"
#include "../sprites/sprites.h"
#include "../game/game_utils.h"
#include "../game/game_play.h"

sprite_t *level_1_background;
sprite_t *level_2_background;
sprite_t *level_3_background;
sprite_t *pause_background;
sprite_t *pause_resume;
sprite_t *pause_main_menu;
sprite_t *win_main_menu;
sprite_t *dead_restart;
sprite_t *pause_button;

/**
 * @brief Retrieves the dimensions of an XPM image.
 * 
 * @param xpm The XPM image.
 * @param width Pointer to store the width of the image.
 * @param height Pointer to store the height of the image.
 */
void get_xpm_dimensions(xpm_map_t xpm, int *width, int *height);

/**
 * @brief Draws a pseudo pixel on the screen.
 * 
 * @param pos The position of the pixel.
 * @param color The color of the pixel.
 * @return 0 on success, non-zero otherwise.
 */
int game_draw_pseudo_pixel(position_t pos, uint32_t color);

/**
 * @brief Draws an XPM image on the screen.
 * 
 * @param xpm The XPM image.
 * @param pos The position to draw the image.
 * @param color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_draw_xpm(xpm_map_t xpm, position_t pos, uint32_t *color_array);

/**
 * @brief Undraws an XPM image from the screen.
 * 
 * @param xpm The XPM image.
 * @param pos The position to undraw the image.
 * @param color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_undraw_xpm(xpm_map_t xpm, position_t pos, uint32_t *color_array);

/**
 * @brief Draws text on the screen.
 * 
 * @param text The text to draw.
 * @param pos The position to draw the text.
 * @param color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_draw_text(char* text, position_t pos, uint32_t *color_array);

/**
 * @brief Draws a sprite on the screen.
 * 
 * @param spr The sprite to draw.
 * @param color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_draw_sprite(sprite_t *spr, uint32_t *color_array);

/**
 * @brief Draws a non-pseudo sprite on the screen.
 * 
 * @param sprite The non-pseudo sprite to draw.
 * @param pos_x The x position to draw the sprite.
 * @param pos_y The y position to draw the sprite.
 * @return 0 on success, non-zero otherwise.
 */
int game_draw_not_pseudo_sprite(not_pseudo_sprite *sprite, int pos_x, int pos_y);

/**
 * @brief Undraws a pseudo pixel from the screen.
 * 
 * @param pos The position of the pixel.
 * @param color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_undraw_pseudo_pixel(position_t pos, uint32_t *color_array);

/**
 * @brief Undraws a non-pseudo sprite from the screen.
 * 
 * @param spr The non-pseudo sprite to undraw.
 * @param last_pos_x The last x position of the sprite.
 * @param last_pos_y The last y position of the sprite.
 * @param game_color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_undraw_not_pseudo_sprite(not_pseudo_sprite *spr, int last_pos_x, int last_pos_y, uint32_t* game_color_array);

/**
 * @brief Undraws a sprite from the screen.
 * 
 * @param spr The sprite to undraw.
 * @param game_color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_undraw_sprite(sprite_t *spr, uint32_t* game_color_array);

/**
 * @brief Creates a color array.
 * 
 * @return Pointer to the created color array.
 */
uint32_t* game_create_color_array();

/**
 * @brief Draws the color array on the screen.
 * 
 * @param game_color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_draw_color_array(uint32_t* game_color_array);

/**
 * @brief Deletes the color array.
 * 
 * @param game_color_array Pointer to the color array.
 */
void game_delete_color_array(uint32_t* game_color_array);

/**
 * @brief Clears the color array.
 * 
 * @param game_color_array Pointer to the color array.
 */
void game_clear_color_array(uint32_t* game_color_array);

/**
 * @brief Adds an XPM image to the color array.
 * 
 * @param xpm The XPM image.
 * @param pos The position to add the image.
 * @param game_color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_add_xpm(xpm_map_t xpm, position_t pos, uint32_t* game_color_array);

/**
 * @brief Adds text to the color array.
 * 
 * @param text The text to add.
 * @param pos The position to add the text.
 * @param game_color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_add_text(char* text, position_t pos, uint32_t* game_color_array);

/**
 * @brief Adds a sprite to the color array.
 * 
 * @param spr The sprite to add.
 * @param game_color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int game_add_sprite(sprite_t *spr, uint32_t* game_color_array);

/**
 * @brief Clears the color array.
 * 
 * @param color_array Pointer to the color array.
 */
void clear_color_array(uint32_t *color_array);

/**
 * @brief Prepares the sprites for rendering.
 */
int prepare_sprites();

/**
 * @brief Deletes the sprites.
 */
void delete_sprites();

/**
 * @brief Prints the mouse on the screen.
 * 
 * @param game_color_array Pointer to the color array.
 */
void print_mouse(uint32_t* game_color_array);

/**
 * @brief Prints the countdown on the screen.
 * 
 * @param countdown The countdown value.
 * @param color_array Pointer to the color array.
 */
void print_countdown(unsigned int countdown, uint32_t *color_array);

/**
 * @brief Prints the minimum points on the screen.
 * 
 * @param min_points The minimum points value.
 * @param color_array Pointer to the color array.
 */
void print_min_points(unsigned int min_points, uint32_t *color_array);

/**
 * @brief Prints the points on the screen.
 * 
 * @param points The current points value.
 * @param prev_points The previous points value.
 * @param color_array Pointer to the color array.
 */
void print_points(unsigned int points, unsigned int prev_points, uint32_t *color_array);

/**
 * @brief Increases the redness of the color array.
 * 
 * @param color_array Pointer to the color array.
 * @param increment The amount to increase the redness.
 */
void increase_redness(uint32_t *color_array, uint8_t increment);

/**
 * @brief Draws the dead text on the screen.
 * 
 * @param death_str The death message.
 * @param color_array Pointer to the color array.
 */
void draw_dead_text(const char *death_str, uint32_t *color_array);

/**
 * @brief Creates a sized color array.
 * 
 * @param x The width of the color array.
 * @param y The height of the color array.
 * @return Pointer to the created color array.
 */
uint32_t* game_create_sized_color_array(int x, int y);

/**
 * @brief Loads an XPM image to the color array.
 * 
 * @param xpm The XPM image.
 * @param pos The position to load the image.
 * @param color_array Pointer to the color array.
 * @return 0 on success, non-zero otherwise.
 */
int load_xpm(xpm_map_t xpm, position_t pos, uint32_t *color_array);

/**
 * @brief Draws a frame array on the screen.
 * 
 * @param color_array Pointer to the color array.
 * @param x The x position of the frame.
 * @param y The y position of the frame.
 * @param size_x The width of the frame.
 * @param size_y The height of the frame.
 * @return 0 on success, non-zero otherwise.
 */
int game_draw_frame_array(uint32_t* color_array, int x, int y, int size_x, int size_y);

#endif
