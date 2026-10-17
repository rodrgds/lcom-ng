#include "Cursor.h"

/**
 * @brief Allocates and initializes a new Cursor instance.
 *
 * Sets up a cursor with the given position and dimensions.
 * The memory is dynamically allocated and must be freed using `delete_Cursor()`.
 *
 * @param position Initial position of the cursor.
 * @param width Width of the cursor in pixels.
 * @param height Height of the cursor in pixels.
 * @return Pointer to the newly created Cursor, or NULL if allocation fails.
 */

Cursor *new_cursor(Position position, int16_t width, int16_t height){
    
    Cursor *c = malloc(sizeof(*c));
    if( c== NULL){
        return NULL;
    }
    c->position = position;
    c->width = width;
    c->height = height;
    return c;
}

/**
 * @brief Frees the memory allocated for a Cursor instance.
 *
 * @param c Pointer to the Cursor to delete.
 */
void delete_Cursor(Cursor *c){
    free(c);
}

/**
 * @brief Retrieves the current position of the Cursor.
 *
 * @param c Pointer to the Cursor.
 * @return The current position of the cursor.
 */

Position get_position(Cursor *c){
    return c->position;
}
