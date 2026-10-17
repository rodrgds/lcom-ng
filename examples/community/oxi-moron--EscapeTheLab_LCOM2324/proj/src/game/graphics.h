#ifndef G6_GRAPHICS_H
#define G6_GRAPHICS_H

#include <lcom/lcf.h>

#include "../drivers/video.h"
#include "map.h"
#include "player.h"
#include "graphics_macros.h"
#include "cursor.h"
#include "clock.h"
#include "intercom.h"
#include <math.h>

#include "../images/main_menu.xpm"
#include "../images/wall_texture.xpm"
#include "../images/cursor.xpm"
#include "../images/victory.xpm"
#include "../images/defeat.xpm"
#include "../images/pause_menu.xpm"
#include "../images/whitebrick.xpm"
#include "../images/player_select.xpm"
#include "../images/doorLazerBad.xpm"
#include "../images/doorLazer.xpm"
#include "../images/letter_L.xpm"
#include "../images/letter_C.xpm"
#include "../images/letter_O.xpm"
#include "../images/letter_M.xpm"
#include "../images/two_dots.xpm"
#include "../images/1.xpm"
#include "../images/2.xpm"
#include "../images/3.xpm"
#include "../images/4.xpm"
#include "../images/5.xpm"
#include "../images/6.xpm"
#include "../images/7.xpm"
#include "../images/8.xpm"
#include "../images/9.xpm"
#include "../images/0.xpm"

static uint32_t width, height, map_width, map_height;

/**
 * @brief Initializes the graphics (sets up the video and map resolutions)
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_construct) ();

/**
 * @brief Draws the current game frame (map, items and player camera).
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_game) ();

/**
 * @brief Draws the main menu screen.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_menu) ();

/**
 * @brief Draws the pause menu screen.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_pause_menu) ();

/**
 * @brief Draws the defeat screen.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_defeat_screen) ();

/**
 * @brief Draws the victory screen.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_victory_screen) ();

/**
 * @brief Draws the intercom screen.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_intercom) ();

/**
 * @brief Draws the player selection screen.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_player_select) ();

/**
 * @brief Draws the in-game timer.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_timer) ();

/**
 * @brief Draws the floor of the game.
 * @return
 */
int (graphics_draw_floor) ();

/**
 * @brief Draws the ceiling of the game.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_ceiling) ();

/**
 * @brief Draws the players camera angle.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_player_camera) ();

/**
 * @brief Draws the game screen map.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_map) ();

/**
 * @brief Draws the in-game cursor.
 * @return 0 if successful, non-zero otherwise.
 */
int (graphics_draw_cursor) ();

/**
 * @brief Loads the initial XPMs
 */
void (load_xpms) ();

/**
 * @brief Creates a line using Bresenham's Line Algorithm to draw the player vision.
 * @param line Address where line will be stored.
 * @param angle Angle between the player and the collision point of its sight.
 * @param delimiter Address where the lines delimiter will be stored (1 if it's a wall, 2 if it's a door).
 * @return Length of the line in number of points.
 */
int (create_line) (struct point2D* line, double angle, uint8_t* delimiter);

/**
 * @brief Sets the screen's background color.
 * @param color Color to be set.
 * @return 0 if successful, non-zero otherwise.
 */
int (set_background_color) (uint32_t color);

/**
 * @brief Calculates the angle for the Line Drawing Algorithm.
 * @param diff The horizontal coordinate of the line to be calculated.
 * @return
 */
double (get_ray_angle) (int diff);

#endif //G6_GRAPHICS_H
