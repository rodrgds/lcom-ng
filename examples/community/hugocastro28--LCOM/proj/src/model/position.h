#ifndef POSITION_H
#define POSITION_H

#include <lcom/lcf.h>

/**
 * @brief Postion of a certain game element.
 * 
 */
typedef struct {
    int left_x;
    int right_x;
    int top_y;
    int bottom_y;
} Position;

#endif
