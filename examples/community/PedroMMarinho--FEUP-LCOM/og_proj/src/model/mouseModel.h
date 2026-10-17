#pragma once
#include "lcom/lcf.h"
#include "vector.h"


/**
 * @brief Struct that represents the mouse
 * 
 */
typedef struct Mouse{
  vector_t pos;
  vector_t savedPos;
  vector_t delta;
}Mouse;

/**
 * @brief Creates a new mouse
 * 
 * @return Mouse* Pointer to the mouse
 */
Mouse* newMouse();

/**
 * @brief Frees the memory allocated for the mouse
 * 
 * @param mouse Mouse to be destroyed
 */
void destroyMouse(Mouse* mouse);
