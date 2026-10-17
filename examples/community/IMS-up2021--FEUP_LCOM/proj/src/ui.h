#ifndef PROJ_UI_H
#define PROJ_UI_H

#include <lcom/lcf.h>
#include <stdbool.h>

/**
 * @brief Represents a button in the graphical user interface.
 */
typedef struct {
    unsigned int pos_x;   /**< The x-coordinate position of the button (constant). */
    unsigned int pos_y;   /**< The y-coordinate position of the button (constant). */
    unsigned int width;  /**< The width of the button (constant). */
    unsigned int height; /**< The height of the button (constant). */
    const uint8_t *sprite;     /**< Pointer to the sprite representing the button (constant). */
    void (*on_click)(void);    /**< Pointer to the function to be executed when the button is clicked. */
} Button;

/**
 * @brief Represents a cursor in the graphical user interface.
 */
typedef struct {
    unsigned int pos_x;         /**< The x-coordinate position of the cursor. */
    unsigned int pos_y;         /**< The y-coordinate position of the cursor. */
    unsigned int width;  /**< The width of the cursor (constant). */
    unsigned int height; /**< The height of the cursor (constant). */
    const uint8_t *sprite;     /**< Pointer to the sprite representing the cursor (constant). */
} Cursor;

/**
 * @brief Represents a digit in the graphical user interface.
 */
typedef struct {
    unsigned int width;  /**< The width of the digit (constant). */
    unsigned int height; /**< The height of the digit (constant). */
    const uint8_t *sprite;     /**< Pointer to the sprite representing the digit (constant). */
} Digit;

/**
 * @brief Creates a button with specified parameters.
 *
 * This function creates a button at the specified position with the provided sprite and 
 * on_click callback function.
 *
 * @param pos_x The x-coordinate position of the button.
 * @param pos_y The y-coordinate position of the button.
 * @param xpm The XPM map representing the sprite of the button.
 * @param on_click Pointer to the function to be executed when the button is clicked.
 * @return Pointer to the created button structure.
 */
Button *create_button(unsigned int pos_x, unsigned int pos_y, xpm_map_t xpm, void (*on_click)(void));

/**
 * @brief Creates a cursor with specified parameters.
 *
 * This function creates a cursor at the specified position with the provided sprite.
 *
 * @param pos_x The x-coordinate position of the cursor.
 * @param pos_y The y-coordinate position of the cursor.
 * @param xpm The XPM map representing the sprite of the cursor.
 * @return Pointer to the created cursor structure.
 */
Cursor *create_cursor(unsigned int pos_x, unsigned int pos_y, xpm_map_t xpm);


#endif
