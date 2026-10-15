#include "Plane.h"

/** @defgroup Plane Plane
* @{
 */


/**
 * @brief Creates an array of Planes Objects.
 * @param maxPlaneNumber Number of planes.
 * @return The created Planes Object.
 * @details Planes is a struct made of an array of Plane Objects (each made of an Object and a state) and its size.
 */
Planes *create_PlanesArray(int maxPlaneNumber){
    Planes* newPlanes = (Planes*)malloc(sizeof(Planes)); 
    newPlanes->plane = (Plane**)malloc(sizeof(Plane) * maxPlaneNumber); 
    newPlanes->sizePlanes = 0;
    return newPlanes;
}



/**
 * @brief Creates a Plane Object, that can appear on either the right or left, and is added to the array of Planes b.
 * @param b Planes which will contain a new Plane.
 * @param left If true plane come from the left, right otherwise
 * @return The new Plane.
 * @see create_object() for used method of creating an object.
 */
Plane* (createPlane)(Planes *b, bool left) {
    Plane* newPlane = (Plane*)malloc(sizeof(Plane));
    newPlane->state = 0;
    if (left) {
        newPlane->planeObject = create_object((xpm_map_t) plane_xpm, 0, 100);
        newPlane->planeObject->xspeed = 3;
        newPlane->planeObject->sprite->x = -newPlane->planeObject->sprite->width;
    }else{
        newPlane->planeObject = create_object((xpm_map_t) planeR_xpm, 0, 100);
        newPlane->planeObject->xspeed = -3;
        newPlane->planeObject->sprite->x = WindowWidth;
    } 
    b->plane[b->sizePlanes] = newPlane;
    b->sizePlanes++;  
    return newPlane;
}

/**
 * @brief Removes a Plane from the Planes array.
 * @param b Planes Object.
 * @param indexToRemove Index of the Plane to be removed.
 */
void (removePlane)(Planes *b, int indexToRemove){
    if (indexToRemove < 0 || indexToRemove >= b->sizePlanes) return;
    free(b->plane[indexToRemove]);
    for (int i = indexToRemove; i < (b->sizePlanes - 1); i++) {
        b->plane[i] = b->plane[i + 1];
    }
    update_score(5);  // Update the score only the first time a plane is shot
    b->sizePlanes--;
}


/**
 * @brief Checks plane movement for all Planes.
 * @param PlanesArray All Planes in the field.
 * @see planeMovement() for individual plane movement.
 */
void (PlanesMovement)(Planes *PlanesArray){
    for (int i = 0; i < PlanesArray->sizePlanes; i++){
        planeMovement(PlanesArray, i);
    }
}

/**
 * @brief Draws Planes from the Planes Object array.
 * @param PlanesArray All Planes in the field.
 * @see drawMultipleSprite() to understand how division of the total xpm works.
 */
void (drawPlanes)(Planes *PlanesArray){
    for (int i = 0; i < PlanesArray->sizePlanes; i++){
        drawMultipleSprite(PlanesArray->plane[i]->state, 3, 1, PlanesArray->plane[i]->planeObject->sprite);
    }
}

/**
 * @brief Checks whether a plane is within bounds and sets speed.
 * @param PlanesArray All Planes in the field.
 * @param index Position of a plane in the Planes array.
 * @see removePlane() for plane removal from array of Planes.
 */
void (planeMovement)(Planes *PlanesArray, int index){
    Plane *plane = PlanesArray->plane[index];
    plane->planeObject->sprite->x += plane->planeObject->xspeed;
    plane->planeObject->sprite->y += plane->planeObject->yspeed;
    if (plane->planeObject->sprite->x < - plane->planeObject->sprite->width ||
    plane->planeObject->sprite->x > WindowWidth || plane->planeObject->sprite->y + plane->planeObject->sprite->height >= 800){
        removePlane(PlanesArray, index);
    } 
}


/**
 * @brief Frees up memory used by planes.
 * @param planes All Planes in the field.
 */
void (freePlanes)(Planes* planes) {
    if (planes == NULL) return;
    for (int i = 0; i < planes->sizePlanes; i++) {
        free(planes->plane[i]->planeObject);
        free(planes->plane[i]);
    }
    free(planes->plane);
    free(planes);
}

/**@}*/
