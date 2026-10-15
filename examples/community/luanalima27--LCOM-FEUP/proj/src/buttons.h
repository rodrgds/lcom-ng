/**
 * @file buttons.h
 * @brief Header file containing function prototypes for button manipulation.
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#include <lcom/lcf.h>

/**
 * @brief Draws the quit button sprite at the specified coordinates.
 * @details This function draws the quit button sprite at the given (x, y) coordinates on the 
 * screen.
 * @param x The x-coordinate of the upper-left corner of the quit button sprite.
 * @param y The y-coordinate of the upper-left corner of the quit button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_quit(uint16_t x, uint16_t y);

/**
 * @brief Draws the start button sprite at the specified coordinates.
 * @details This function draws the start button sprite at the given (x, y) coordinates on the 
 * screen.
 * @param x The x-coordinate of the upper-left corner of the start button sprite.
 * @param y The y-coordinate of the upper-left corner of the start button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_start(uint16_t x, uint16_t y);
int draw_back(uint16_t x, uint16_t y);
int draw_easy(uint16_t x, uint16_t y);
int draw_medium(uint16_t x, uint16_t y);
int draw_hard(uint16_t x, uint16_t y);


/**
 * @brief Draws the selected start button sprite at the specified coordinates.
 * @details This function draws the selected start button sprite at the given (x, y) coordinates on 
 * the screen.
 * @param x The x-coordinate of the upper-left corner of the selected start button sprite.
 * @param y The y-coordinate of the upper-left corner of the selected start button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_selected_start(uint16_t x, uint16_t y);

/**
 * @brief Draws the selected quit button sprite at the specified coordinates.
 * @details This function draws the selected quit button sprite at the given (x, y) coordinates on 
 * the screen.
 * @param x The x-coordinate of the upper-left corner of the selected quit button sprite.
 * @param y The y-coordinate of the upper-left corner of the selected quit button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_selected_quit(uint16_t x, uint16_t y);

/**
 * @brief Draws the selected back button sprite at the specified coordinates.
 * @details This function draws the selected back button sprite at the given (x, y) coordinates on 
 * the screen.
 * @param x The x-coordinate of the upper-left corner of the selected back button sprite.
 * @param y The y-coordinate of the upper-left corner of the selected back button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_selected_back(uint16_t x, uint16_t y);

/**
 * @brief Draws the selected easy button sprite at the specified coordinates.
 * @details This function draws the selected easy button sprite at the given (x, y) coordinates on 
 * the screen.
 * @param x The x-coordinate of the upper-left corner of the selected easy button sprite.
 * @param y The y-coordinate of the upper-left corner of the selected easy button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_selected_easy(uint16_t x, uint16_t y);

/**
 * @brief Draws the selected medium button sprite at the specified coordinates.
 * @details This function draws the selected medium button sprite at the given (x, y) coordinates on 
 * the screen.
 * @param x The x-coordinate of the upper-left corner of the selected medium button sprite.
 * @param y The y-coordinate of the upper-left corner of the selected medium button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_selected_medium(uint16_t x, uint16_t y);

/**
 * @brief Draws the selected hard button sprite at the specified coordinates.
 * @details This function draws the selected hard button sprite at the given (x, y) coordinates on 
 * the screen.
 * @param x The x-coordinate of the upper-left corner of the selected hard button sprite.
 * @param y The y-coordinate of the upper-left corner of the selected hard button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_selected_hard(uint16_t x, uint16_t y);


/**
 * @brief Deletes the quit button sprite at the specified coordinates.
 * @details This function deletes the quit button sprite at the given (x, y) coordinates on the 
 * screen by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the quit button sprite.
 * @param y The y-coordinate of the upper-left corner of the quit button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_quit(uint16_t x, uint16_t y);

/**
 * @brief Deletes the start button sprite at the specified coordinates.
 * @details This function deletes the start button sprite at the given (x, y) coordinates on the 
 * screen by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the start button sprite.
 * @param y The y-coordinate of the upper-left corner of the start button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_start(uint16_t x, uint16_t y);

/**
 * @brief Deletes the back button sprite at the specified coordinates.
 * @details This function deletes the back button sprite at the given (x, y) coordinates on the 
 * screen by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the back button sprite.
 * @param y The y-coordinate of the upper-left corner of the back button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_back(uint16_t x, uint16_t y);

/**
 * @brief Deletes the back button sprite at the specified coordinates.
 * @details This function deletes the back button sprite at the given (x, y) coordinates on the 
 * screen by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the back button sprite.
 * @param y The y-coordinate of the upper-left corner of the back button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_easy(uint16_t x, uint16_t y);

/**
 * @brief Deletes the medium button sprite at the specified coordinates.
 * @details This function deletes the medium button sprite at the given (x, y) coordinates on the 
 * screen by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the medium button sprite.
 * @param y The y-coordinate of the upper-left corner of the medium button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_medium(uint16_t x, uint16_t y);

/**
 * @brief Deletes the hard button sprite at the specified coordinates.
 * @details This function deletes the hard button sprite at the given (x, y) coordinates on the 
 * screen by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the hard button sprite.
 * @param y The y-coordinate of the upper-left corner of the hard button sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_hard(uint16_t x, uint16_t y);


/**
 * @brief Draws the win message sprite at the specified coordinates.
 * @details This function draws the win message sprite at the given (x, y) coordinates on the 
 * screen.
 * @param x The x-coordinate of the upper-left corner of the win message sprite.
 * @param y The y-coordinate of the upper-left corner of the win message sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_win(uint16_t x, uint16_t y);

/**
 * @brief Draws the game over message sprite at the specified coordinates.
 * @details This function draws the game over message sprite at the given (x, y) coordinates on the 
 * screen.
 * @param x The x-coordinate of the upper-left corner of the game over message sprite.
 * @param y The y-coordinate of the upper-left corner of the game over message sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_gameover(uint16_t x, uint16_t y);


/**
 * @brief Deletes the game over message sprite at the specified coordinates.
 * @details This function deletes the game over message sprite at the given (x, y) coordinates on 
 * the screen by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the game over message sprite.
 * @param y The y-coordinate of the upper-left corner of the game over message sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_game_over(uint16_t x, uint16_t y);


/**
 * @brief Deletes the win message sprite at the specified coordinates.
 * @details This function deletes the win message sprite at the given (x, y) coordinates on the 
 * screen by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the win message sprite.
 * @param y The y-coordinate of the upper-left corner of the win message sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_win(uint16_t x, uint16_t y);

#endif
