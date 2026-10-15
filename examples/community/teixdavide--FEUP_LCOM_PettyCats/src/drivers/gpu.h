/**
 * @file gpu.h
 * @brief This file contains all the essential functins to operate MINIX in video mode
 * @date 2024-06-02
 * 
 */

#ifndef _GPU_H_
#define _GPU_H_

#define INTNO 0x10

#include <string.h>

#include "../font/characters.xpm"
#include "../models/Sprite.h"


/**
 * @brief Initializes video mode in the selected mode
 * 
 * @param mode 
 * @return 0 upon success, 1 otherwise 
 */
int (enter_video_mode)(uint16_t mode);


/**
 * @brief Exits video mode
 * 
 * @return 0 upon success, 1 otherwise
 * 
 */
int (exit_video_mode)();


/**
 * @brief Creates a vram buffer 
 * 
 * @param mode selected mode
 * @return 0 upon success, 1 otherwise 
 */
int (create_vram_buffer)(uint16_t mode);


/**
 * @brief Draws a pixel on the screen
 * 
 * @param mode selected mode
 * @param x x axis position
 * @param y y axis position
 * @param color selected color
 * @return 0 upon success, 1 otherwise 
 */
int (draw_pixel)(uint16_t mode, uint16_t x, uint16_t y, uint32_t color);


/**
 * @brief Draws a line on the screen
 * 
 * @param mode selected mode
 * @param x x axis position
 * @param y y axis position
 * @param width line size
 * @param color selected color
 * @return 0 upon success, 1 otherwise 
 */
int (draw_line)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint32_t color);


/**
 * @brief Draws a rectangle on the screen
 * 
 * @param mode selected video mode
 * @param x x axis position
 * @param y y axis position
 * @param width rectangle width
 * @param height rectangle height
 * @param color selected color
 * @return 0 upon success, 1 otherwise 
 */
int (draw_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint32_t color);


/**
 * @brief Draws a xpm map
 * 
 * @param xpm xpm map to be drawn
 * @param x x axis position
 * @param y y axis position
 * @return 0 upon success, 1 otherwise 
 */
int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);


/**
 * @brief Draws a sprite
 * 
 * @param sprite sprite to be drawn
 * @param x x axis position
 * @param y y axis position
 * @return 0 upon success, 1 otherwise 
 */
int (draw_sprite)(Sprite sprite, uint16_t x, uint16_t y);


/**
 * @brief Draws a sprite in a specific color only 
 * 
 * @param sprite sprite to be drawn
 * @param x x axis position
 * @param y y axis position
 * @param color color in which the sprite will be drawn
 * @return 0 upon success, 1 otherwise 
 */
int (draw_sprite_single_color)(Sprite sprite, uint16_t x, uint16_t y, uint32_t color);


/**
 * @brief Get the index of a character sprite
 * 
 * @param ch selected character
 * @return the index if found, 0 if not found 
 */
int (get_char_sprite_idx)(char ch);


/**
 * @brief Renders text on the screen
 * 
 * @param text 
 * @param x x axis position
 * @param y y axis position
 * @param color chosen color
 * @return 0 upon success, 1 otherwise 
 */
int (draw_text)(const char *text, uint16_t x, uint16_t y, uint32_t color);


/**
 * @brief Set the screen's background to selected color
 * 
 * @param mode selected mode
 * @param color selected color
 * @return 0 upon success, 1 otherwise 
 */
int (set_background_color)(uint16_t mode, uint32_t color);


/**
 * @brief Copies memory of back buffer to front buffer
 * 
 * @returns 0 upon success, 1 otherwise
 * 
 */
int (update_front_buffer)();

#endif
