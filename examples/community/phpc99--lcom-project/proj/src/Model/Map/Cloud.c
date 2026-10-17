#include "Cloud.h"

/**
 * @brief Allocates and initializes a new Cloud instance.
 *
 * Creates a cloud with a given position and size.
 * Memory is allocated dynamically.
 *
 * @param position Initial position of the cloud.
 * @param width Width of the cloud in pixels.
 * @param height Height of the cloud in pixels.
 * @return Pointer to the newly created Cloud, or NULL if allocation fails.
 */

Cloud *new_Cloud(Position position, int16_t width, int16_t height){
    Cloud *cl = malloc(sizeof(*cl));

    if( cl == NULL){
        return NULL;
    }
    cl->position = position;
    cl->width = width;
    cl->height = height;

    
    return cl;
}

/**
 * @brief Frees the memory allocated for a Cloud instance.
 *
 * @param cl Pointer to the Cloud instance to be deleted.
 */

void delete_Cloud(Cloud *cl){
    free(cl);
}
