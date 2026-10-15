//
// Created by Matilde Sequeira on 12/06/2022.
//

#ifndef SHARED_APPLE_H
#define SHARED_APPLE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <lcom/lcf.h>

/**
 * @brief enum with the different types of apples
 */
enum appleType{red, brown, black};

/**
 * @brief struct of an apple
 *
 */
typedef struct {
    int x_pos;
    int y_pos;
    object appleXpm;
    enum appleType type;
}Apple;

/** @brief array of the apples on game */
Apple applesArray[195];

/** @brief number of apples on game */
int numApples;


/**
 * @brief red apple object*/
object redAppleXpm;
/**
 * @brief brown apple object*/
object brownAppleXpm;
/**
 * @brief black apple object
 * */
object blackAppleXpm;
/**
 * @brief background object
 * */
object background;


/**
 * @brief initiates the apples xpms
 */
void initAppleXPM();

/**
 * @brief Creates an apple and puts it on the array
 *
 * @param apple struck of the object Apple
 * @param x_pos column where to place the apple
 * @param y_pos line where to place the apple
 * @param type enum appleType of the apple
 */
void initApple(Apple *apple, int x_pos, int y_pos, enum appleType type);
/**
 * @brief places an apple in a random place
 *
 * @param apple struck of the object Apple
 * @param type enum appleType of the apple
 */
void initRandomApple(Apple *apple, enum appleType type);
/**
 * @brief Gets the xpm object by the apple type
 *
 * @param type appleType enum
 * @return object the apple xpm
 */
object get_xpm(enum appleType type);
/**
 * @brief Draws an apple
 *
 * @param apple to be drawn
 */
void drawApple(Apple apple);
/**
 * @brief Verifies if there is an apple in the position passed by parameter
 *
 * @param x position (column) to be checked
 * @param y position (line) to be checked
 * @return int -1 if theres not an apple, or the position in the array
 */
int isApple(int x, int y);
/**
 * @brief places an apple within a square of the background
 *
 * @param x column of the position where the apple has to be placed
 * @param y line of the position where to apple has to be placed
 * @param type enum appleType of the apple
 */
void placeApple(int x, int y, enum appleType type);


#endif //SHARED_APPLE_H
