#include "Object.h"

/** @defgroup Object Object
* @{
 */

/**
 * @brief Draws an object, drawing the sprite associated with the object.
 * @param Object
 * @see drawSprite() to understand how sprite drawing works.
 */
void drawObject(Object *Object){
    drawSprite(Object->sprite);
}

/**
 * @brief Creates an object with no speed. Uses sprite creation for the sprite parameter.
 * @param xpm 
 * @param x Position on the x axis.
 * @param y Position on the y axis.
 * @return The new object.
 *  * @see create_sprite() for used method of creating a sprite.
 */
Object *create_object(xpm_map_t xpm, int x, int y){
    Object *obj = (Object *) malloc (sizeof(Object));
    if (obj != NULL){
        obj->sprite = create_sprite(xpm, x, y);
        obj->xspeed = 0;
        obj->yspeed = 0;
    }
    return obj;
}
/**@}*/


