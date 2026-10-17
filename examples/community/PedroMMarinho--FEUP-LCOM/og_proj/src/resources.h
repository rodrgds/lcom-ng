#pragma once

#include "lcom/lcf.h"
#include "model/table.h"
#include "States/states.h"
#include "model/menu.h"

/**
 * @brief Struct that contains the resources of the game
 * 
 */
typedef struct Resources
{
  Table* table;
  Menu* menu;
  STATE state;
  char playerName[20];
} Resources;

/**
 * @brief Loads the resources of the game
 * 
 * @return Resources* Pointer to the resources
 */
Resources * loadResources();

/**
 * @brief Destroys the resources of the game
 * 
 * @param resources Resources to be destroyed
 */
void destroyResources(Resources* resources);

