#include "Sun.h"

/**
 * @brief Allocates and initializes a new Sun instance.
 *
 * Creates a new Sun structure with the specified position and dimensions.
 * Memory is dynamically allocated.
 *
 * @param position Initial position of the sun.
 * @param width Width of the sun in pixels.
 * @param height Height of the sun in pixels.
 * @return Pointer to the newly created Sun object, or NULL if allocation fails.
 */

Sun *new_Sun(Position position, int16_t width, int16_t height){
    Sun *sn = malloc(sizeof(*sn));

    if( sn == NULL){
        return NULL;
    }
    sn->position = position;
    sn->width = width;
    sn->height = height;

    
    return sn;

}

/**
 * @brief Frees the memory allocated for a Sun instance.
 *
 * @param sn Pointer to the Sun instance to be deleted.
 */

void delete_Sun(Sun *sn){
    free(sn);
}
