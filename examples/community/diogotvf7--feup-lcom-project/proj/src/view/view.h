#pragma once

#include <lcom/lcf.h>
#include "controller/mouse/mouse.h"
#include "controller/graphics_card/graphics_card.h"
#include "model/sprite.h"
#include "model/colors.h"
#include "model/model.h"


/**
 * @brief Draws a new frame on the screen.
 */
void draw_new_frame();

/**
 * @brief Draws the mouse cursor on the screen.
 */
void draw_mouse();

/**
 * @brief Draws the initial menu on the screen.
 */
void draw_initial_menu();

/**
 * @brief Draws the game menu on the screen.
 */
void draw_game_menu();

/**
 * @brief Draws the leaderboard on the screen.
 */
void draw_leaderboard();

/**
 * @brief Draws the end menu on the screen.
 */
void draw_end_menu();

/**
 * @brief Draws a sprite on the screen at the specified coordinates.
 *
 * @param sprite The sprite to draw.
 * @param x The x-coordinate of the sprite's position.
 * @param y The y-coordinate of the sprite's position.
 * @return 0 on success, non-zero on failure.
 */
int draw_sprite_xpm(Sprite* sprite, int x, int y);

/**
 * @brief Draws a bar on the screen with the specified dimensions and color.
 *
 * @param x The x-coordinate of the bar's position.
 * @param y The y-coordinate of the bar's position.
 * @param width The width of the bar.
 * @param height The height of the bar.
 * @param color The color of the bar.
 * @return 0 on success, non-zero on failure.
 */
int draw_bar(int x, int y, int width, int height, uint32_t color);

/**
 * @brief Draws a bottom bar on the screen with the specified dimensions, color, and square parameters.
 *
 * @param x The x-coordinate of the bottom bar's position.
 * @param y The y-coordinate of the bottom bar's position.
 * @param width The width of the bottom bar.
 * @param height The height of the bottom bar.
 * @param color The color of the bottom bar.
 * @param square_vertex_x The x-coordinate of the square's vertex within the bottom bar.
 * @param square_vertex_y The y-coordinate of the square's vertex within the bottom bar.
 * @param square_width The width of the square.
 * @param square_height The height of the square.
 * @return 0 on success, non-zero on failure.
 */
int draw_bottom_bar(int x, int y, int width, int height, uint32_t color, int square_vertex_x, int square_vertex_y, int square_width, int square_height);

/**
 * @brief Draws the game time on the screen.
 *
 * @param num The game time to be displayed.
 * @return 0 on success, non-zero on failure.
 */
int draw_game_time(int num);

/**
 * @brief Draws a letter on the screen at the specified coordinates with the given color.
 *
 * @param x The x-coordinate of the letter's position.
 * @param y The y-coordinate of the letter's position.
 * @param letter_index The index of the letter.
 * @param color The color of the letter.
 * @return 0 on success, non-zero on failure.
 */
int draw_letter(int x, int y, uint8_t letter_index, uint32_t color);

/**
 * @brief Draws a number on the screen at the specified coordinates using a sprite and index.
 *
 * @param sprite The sprite containing the number images.
 * @param x The x-coordinate of the number's position.
 * @param y The y-coordinate of the number's position.
 * @param index The index of the number to be displayed.
 * @return 0 on success, non-zero on failure.
 */
int draw_number(Sprite* sprite, int x, int y, int index);

/**
 * @brief Draws a word on the screen at the specified coordinates with the given color.
 *
 * @param word An array of integers representing the word.
 * @param word_size The size of the word array.
 * @param x The x-coordinate of the word's position.
 * @param y The y-coordinate of the word's position.
 * @param color The color of the word.
 * @return 0 on success, non-zero on failure.
 */
int draw_word(uint8_t word[], int word_size, int x, int y, uint32_t color);


