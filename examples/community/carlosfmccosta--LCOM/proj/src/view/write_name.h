/**
 * @file write_name.h
 * @brief Declares the write name page UI and its operations.
 *
 * Provides the interface for creating, destroying, and drawing the write name page,
 * as well as accessing and modifying the player's name, options, callbacks, and cursor position.
 */

#ifndef __WRITE_NAME_H
#define __WRITE_NAME_H

#include "model/sprite.h"
#include "text.h"
#include "assets/ui.h"

/**
 * @brief Struct representing the write name page.
 */
struct write_name;
typedef struct write_name write_name_t;

/** @brief Maximum number of characters allowed for the player's name. */
#define MAX_NAME_CHARS 10

/**
 * @brief Creates a write name page.
 * @param title Title string for the page.
 * @param cue Cue string ("Write your name:").
 * @param options Array of option strings.
 * @param callbacks Array of callback functions for each option.
 * @param size Number of options.
 * @param h_res Horizontal resolution of the screen.
 * @param v_res Vertical resolution of the screen.
 * @return Pointer to the created write name page, or NULL on failure.
 */
write_name_t *create_write_name_page(const char *title, const char *cue, const char **options, void (**callbacks)(void), unsigned int size, unsigned h_res, unsigned v_res);

/**
 * @brief Draws the write name page on the screen.
 * @param page Pointer to the write name page.
 * @return 0 on success, non-zero on error.
 */
int draw_write_name_page(write_name_t *page);

/**
 * @brief Destroys a write name page and frees its resources.
 * @param page Pointer to the write name page to destroy.
 */
void destroy_write_name_page(write_name_t *page);

/**
 * @brief Gets the player's name entered on the page.
 * @param page Pointer to the write name page.
 * @return Pointer to the player's name string.
 */
char* get_player_name(write_name_t *page);

/**
 * @brief Checks if the page is currently in writing mode.
 * @param page Pointer to the write name page.
 * @return true if writing, false otherwise.
 */
bool is_writing(write_name_t *page);

/**
 * @brief Sets the writing mode of the page.
 * @param page Pointer to the write name page.
 * @param writing true to enable writing mode, false to disable.
 */
void set_page_writing(write_name_t *page, bool writing);

/**
 * @brief Gets the Y position of the page title.
 * @param page Pointer to the write name page.
 * @return Y position in pixels.
 */
uint16_t get_page_title_y(const write_name_t *page);

/**
 * @brief Gets the sprite for the menu option background.
 * @param page Pointer to the write name page.
 * @return Pointer to the option sprite.
 */
sprite_t *get_page_option(write_name_t *page);

/**
 * @brief Gets the callback function for a given menu option.
 * @param page Pointer to the write name page.
 * @param opt Option index.
 * @return Function pointer for the option's callback.
 */
void (*get_page_callback(write_name_t *page, unsigned int opt))(void);

/**
 * @brief Gets the number of options in the write name page.
 * @param page Pointer to the write name page.
 * @return Number of options.
 */
unsigned int get_page_size(write_name_t *page);

/**
 * @brief Sets the currently selected menu option index.
 * @param page Pointer to the write name page.
 * @param opt Index to set as selected.
 */
void set_page_opt(write_name_t *page, int opt);

/**
 * @brief Gets the currently selected menu option index.
 * @param page Pointer to the write name page.
 * @return Index of the selected option.
 */
unsigned int get_page_opt(write_name_t *page);

/**
 * @brief Gets the current length of the player's name.
 * @param page Pointer to the write name page.
 * @return Length of the name.
 */
unsigned int get_name_lenght(const write_name_t *page);

/**
 * @brief Sets the current length of the player's name.
 * @param page Pointer to the write name page.
 * @param len Length to set.
 */
void set_name_lenght(write_name_t *page, unsigned int len);

/**
 * @brief Gets the position for the cursor on the write name page.
 * @param page Pointer to the write name page.
 * @return Pointer to the cursor position.
 */
position_t *get_page_cur_pos(write_name_t *page);

/**
 * @brief Gets the text object used for rendering on the write name page.
 * @param page Pointer to the write name page.
 * @return Pointer to the text object.
 */
text_t *get_page_text(write_name_t *page);

#endif //__WRITE_NAME_H
