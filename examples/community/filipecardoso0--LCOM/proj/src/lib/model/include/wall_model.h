#ifndef WALL_MODEL_H
#define WALL_MODEL_H

#include "position.h"

struct wall {
  position_t* position;
};

typedef struct wall wall_t;

/**
 * @brief Generates a new Wall in the specified position.
 * 
 * @param position Input position where the wall will be generated.
 * @return wall_t* that points to wall created.
 */
wall_t* wall_new(position_t* position);

/**
 * @brief Removes a wall from the map.
 * 
 * @param wall Points to the wall that will be deleted.
 */
void wall_delete(wall_t* wall);

/**
 * @brief Sets a wall in the specified position.
 * 
 * @param wall Points to the wall that will have its position updated.
 * @param position Wall's new position.
 */
void wall_set_position(wall_t* wall, position_t* position);

/**
 * @brief Returns the input wall's position.
 * 
 * @param wall Wall that will retrieve its position.
 * @return position_t* Points to the wall's position.
 */
position_t* wall_get_position(wall_t* wall);

#endif
