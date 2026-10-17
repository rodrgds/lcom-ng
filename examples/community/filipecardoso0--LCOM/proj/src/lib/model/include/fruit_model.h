#ifndef FRUIT_MODEL_H
#define FRUIT_MODEL_H

#include "position.h"

/**
 * @brief Struct that represents a fruit. It contains the fruit's position.
 * 
 */
typedef struct {
  position_t* position;
} fruit_t;

/**
 * @brief Creates a new fruit with the specified position.
 * 
 * @param position Position where the new fruit will be spawned.
 * @return fruit_t* Points to the new fruit created.
 */
fruit_t* fruit_new(position_t* position);

/**
 * @brief Deletes a fruit.
 * 
 * @param fruit Points to the fruit to be deleted.
 */
void fruit_delete(fruit_t* fruit);

/**
 * @brief Defines a new position to the specified fruit.
 * 
 * @param fruit Points to the fruit that will get its position updated.
 * @param position Points to the new fruit's position.
 */
void fruit_set_position(fruit_t* fruit, position_t* position);

/**
 * @brief Returns the fruit's position.
 * 
 * @param fruit Points to the fruit that will retrive its position.
 * @return position_t* Points to the fruit's position.
 */
position_t* fruit_get_position(fruit_t* fruit);

#endif
