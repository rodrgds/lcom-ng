#include <lcom/lcf.h>

#include "game_macros.h"
#include "game_structs.h"


/**
 * @brief Checks if two objects collide.
 * For a collision to be detected the area occupied by both objects should intersect.
 * The area of collision of an object is the area occupied by said object.
 * 
 * @param a1 - Area of collision of the first object
 * @param a2 - Area of collision of the second object
 * @return int 
 */
int check_collision(Area_Collision *a1,Area_Collision *a2);

/**
 * @brief Creates a new Area_Collision object.
 * 
 * @param x0 - Leftmost x coordinate
 * @param x1 - Rightmost x coordinate
 * @param y0 - Uppermost y coordinate
 * @param y1 - Lowermost y coordinate
 * @return Area_Collision* - Area_Collision pointer to the area of collision object created
 */
Area_Collision* newAreaCollision(int x0,int x1,int y0,int y1);

/**
 * @brief Deletes a certain area of collision.
 * 
 * @param ac - Area_Collision pointer to the Area_Collision struct object that cointains all the necessary information about the area of collision
 */
void deleteAreaCollision(Area_Collision* ac);


