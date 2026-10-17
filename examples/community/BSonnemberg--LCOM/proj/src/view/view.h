#ifndef VIEW_H
#define VIEW_H

#include "pool.h"

/**
 * @brief Initialize the view by
 * loading all game resources
 * @return non-0 in case of an xpm
*/
int init_view();

/**
 * @brief Stop (unload) the view
 * @return non-0 in case of a fail
*/
int stop_view();

/**
 * @brief Render the menu view
 * @return non-0 in case of a fail
*/
int render_menu_view();

/**
 * @brief Render the game view
 * @param game game to load view from
 * @return non-0 in case of a fail
*/
int render_game_view(pool_game *game, vec_2d mouse);

/**
 * @brief Render the about menu view
 * @return non-0 in case of a fail
*/
int render_about_view();

#endif
