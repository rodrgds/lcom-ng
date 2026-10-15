/**
 * @file Position.h
 * @author your name (you@domain.com)
 * @brief This file contains a struct and two simple functions to create and delete positions
 * @version 0.1
 * @date 2024-06-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _POSITION_H_
#define _POSITION_H_

typedef struct {
    double x;
    double y;
} Position;

/**
 * @brief Creates a new position placed x in x axis and y in y axis
 * 
 * @param x x axis position
 * @param y y axis position
 * @return pointer to new position 
 */
Position *newPosition(int x, int y);
/**
 * @brief Frees memory allocated to previously created position
 * 
 * @param position position to delete
 */
void deletePosition(Position* position);

#endif
