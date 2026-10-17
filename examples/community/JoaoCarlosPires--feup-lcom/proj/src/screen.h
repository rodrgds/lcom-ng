#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Draws in the position (x,y) the xpm passed as argument
 *
 * @param xpm xpm_map_t variable
 * @param x coordinate x where the xpm will be draw
 * @param y coordinate y where the xpm will be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Draws in the position (x,y) the xpm passed as argument but only the non-white pixels
 *
 * @param xpm xpm_map_t variable
 * @param x coordinate x where the xpm will be draw
 * @param y coordinate y where the xpm will be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_xpm_transparent(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Draws in the position (x,y) the xpm passed as argument, without loading it (already loaded)
 *
 * @param xpm xpm_map_t variable
 * @param x coordinate x where the xpm will be draw
 * @param y coordinate y where the xpm will be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_xpm_partial(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Draws the home screen
 *
 * @return Return 1 if the user selected Exit option and non-one otherwise
 */
int home_screen();

/**
 * @brief Draws the game screen
 *
 */
void game_screen();

/**
 * @brief Draws the game over screen
 *
 */
void gameover_screen();

/**
 * @brief Draws the time of the game
 *
 * @param t variable to control time
 */
void draw_time(int t);

/**
 * @brief Draws the cannon of position c
 *
 * @param c variable to set the cannon position
 */
void change_cannon(int c);

/**
 * @brief Draws the man movement in position c 
 *
 * @param c variable to set the man position
 */
void draw_man(int c);

/**
 * @brief Draws the man movement 1
 *
 */
void draw_man_1();

/**
 * @brief Draws the man movement 2
 *
 */
void draw_man_2();

/**
 * @brief Draws the man movement 3
 *
 */
void draw_man_3();

/**
 * @brief Draws the man movement 4
 *
 */
void draw_man_4();

/**
 * @brief Draws the man movement 5
 *
 */
void draw_man_5();

/**
 * @brief Draws the target of level tg
 *
 * @param tg variable to set the target position
 */
void draw_target(int tg);

/**
 * @brief Draws the instructions menu
 *
 */
void instructions_menu();

/**
 * @brief Draws the paused screen
 *
 */
void paused();

/**
 * @brief Draws the level passed as argument
 *
 * @param l variable to set the current level
 */
void draw_level(int l);

/**
 * @brief Draws the score passed as argument
 *
 * @param s variable to set the current score
 */
void draw_score(int s);


#endif
