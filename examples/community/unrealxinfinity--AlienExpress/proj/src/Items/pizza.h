#ifndef PIZZA_H
#define PIZZA_H

#include "../macros/includes.h"
#include "../load/view.h"

/**
 * @brief Calls drawing function to draw pizza
 * @param index Index position of pizza to draw
 */
void draw_pizza(int index);

/**
 * @brief Sets the current inventory position to a pizza.
 * @param index Current inventory index position
 * 
 */
void get_pizza(int index);

/**
 * @brief Raises player's health and stops pizza from being drawn
 * @param index Index position of pizza to consume
 */
void use_pizza(int index);

#endif

