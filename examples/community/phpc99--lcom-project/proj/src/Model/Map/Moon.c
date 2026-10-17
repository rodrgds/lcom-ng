#include "Moon.h"

/**
 * @brief Allocates and initializes a new Moon instance.
 *
 * Creates Moon structure with the specified position and size.
 * Object is allocated dynamically.
 *
 * @param position Initial position of the moon.
 * @param width Width of the moon in pixels.
 * @param height Height of the moon in pixels.
 * @return Pointer to the newly created Moon object, or NULL if memory allocation fails.
 */

Moon *new_Moon(Position position, int16_t width, int16_t height){
    Moon *mn = malloc(sizeof(*mn));

    if( mn == NULL){
        return NULL;
    }
    mn->position = position;
    mn->width = width;
    mn->height = height;

    
    return mn;

}

/**
 * @brief Frees memory allocated for a Moon instance.
 *
 * @param mn Pointer to the Moon instance to be deleted.
 */

void delete_Moon(Moon *mn){
    free(mn);
}
