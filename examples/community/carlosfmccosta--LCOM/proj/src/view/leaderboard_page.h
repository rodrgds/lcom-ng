/**
 * @file leaderboard_page.h
 * @brief Declares the leaderboard page UI and its operations.
 *
 * Provides the interface for creating, destroying, and drawing the leaderboard page,
 * as well as accessing its menu option and callback function.
 */

#ifndef __LB_PAGE
#define __LB_PAGE

#include "model/sprite.h"
#include "assets/numbers.h"
#include "text.h"
#include "leaderboard.h"

/**
 * @brief Struct representing the leaderboard page.
 */
struct lb_page;
typedef struct lb_page lb_page_t;

/**
 * @brief Creates a leaderboard page.
 * @param h_res Horizontal resolution of the screen.
 * @param v_res Vertical resolution of the screen.
 * @param func Callback function for the menu option.
 * @return Pointer to the created leaderboard page, or NULL on failure.
 */
lb_page_t *create_lb_page(unsigned int h_res, unsigned int v_res, void (*func)(void));

/**
 * @brief Destroys a leaderboard page and frees its resources.
 * @param lb_page Pointer to the leaderboard page to destroy.
 */
void destroy_lb_page(lb_page_t *lb_page);

/**
 * @brief Draws the leaderboard page on the screen.
 * @param lb_page Pointer to the leaderboard page.
 * @return 0 on success, non-zero on error.
 */
int draw_leaderboard(lb_page_t *lb_page);

/**
 * @brief Gets the callback function associated with the leaderboard page.
 * @param lb_page Pointer to the leaderboard page.
 * @return Function pointer for the menu option.
 */
void (*get_lb_func(lb_page_t* lb_page))(void);

/**
 * @brief Gets the menu option sprite for the leaderboard page.
 * @param lb_page Pointer to the leaderboard page.
 * @return Pointer to the option sprite.
 */
sprite_t* get_lb_option(lb_page_t* lb_page);

#endif //__LB_PAGE
