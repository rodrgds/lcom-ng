#ifndef PIXMAP_INIT_H
#define PIXMAP_INIT_H

/**
 * @brief Initializes the pixmap used to draw the maze.
 * 
 */
void (initialize_maze_pixmap)();

/**
 * @brief Initializes the pixmap used to draw the pacman.
 * 
 */
void (initialize_pacman_pixmap)();

/**
 * @brief Initializes the pixmap used to draw the ghosts.
 * 
 */
void (initialize_ghost_pixmaps)();

/**
 * @brief Initializes the pixmap used to draw the coins.
 * 
 */
void (initialize_coin_pixmaps)();

/**
 * @brief Initializes the pixmaps used to draw the menu options.
 * 
 */
void (initialize_menu_button_pixmaps)();

/**
 * @brief Initializes the pixmaps used to draw the digits and the mouse cursor.
 * 
 */
void (initialize_util_pixmaps)();

/**
 * @brief Initializes the pixmaps of all elements.
 * 
 */
void (initialize_all_pixmaps)();

/**
 * @brief Draws all the game elements.
 * 
 */
void (draw_game_elements)();

#endif
