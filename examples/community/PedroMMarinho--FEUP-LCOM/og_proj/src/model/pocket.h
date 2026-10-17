#pragma once
#include <lcom/lcf.h>
#include "vector.h"

/**
 * @brief Struct that represents the pocket
*/
typedef struct Pocket{

  vector_t position;
  int radius;

}Pocket;
/**
 * @brief Creates a new pocket
 * 
 * @param pos
 * @param radius
 * @return Pocket*
*/

Pocket* newPocket(vector_t pos, int radius);


