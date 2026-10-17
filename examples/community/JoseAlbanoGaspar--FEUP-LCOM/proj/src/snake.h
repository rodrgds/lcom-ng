#pragma once
#include <lcom/lcf.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "macros/i8042.h"
#include "macros/constants.h"
#include "vbe.h"

/**
 * @brief stores all the stuff related to the snake
 */
struct Snake {
    int direction; //0 right, 1 left, 2 up, 3 down
    int segments_len;
    int segments[2400];
    bool addToSnake;
    bool canChangeDir;
    bool alive;
    int score;
};
/**
 * @brief stores the initial and final horizontal and vertical positions of the enemy,
 * and still stores if he's current active or not
 */
struct Enemy {
    int x;
    int y;
    int lastX;
    int lastY;
    bool active;
};
/**
 * @brief stores the horizontal and vertical positions of the apple
 */
struct Apple {
    int x;
    int y;
};


/**
 * @brief initializes the position of game elements
 * @param vbe_mode video mode which we're in
 */
void (startPosition)(uint16_t vbe_mode);
/**
 * @brief draws the snake on the screen
 */
void (drawSnake)();
/**
 * @brief detects if snake have to change direction based on the pressed key
 * @param scancode scancode of the pressed key
 */
void (changeDirection)(uint16_t scancode);
/**
 * @brief checks if the snake can move without die
 * @param dir direction of the snake movement
 * @return return true if there's no problem on moving on or false otherwise
 */
bool (canMove)(int dir);
/**
 * @brief moves the snake on the screen
 * @param dir direction of the movement
 */
void (moveSnake)(int dir);
/**
 * @brief draws background of te game based on the video mode
 */
void (drawBackground)();
/**
 * @brief aux function for draw the score numbers on the screen
 * @param value score value
 * @param x x_pos to start draw the numbers
 * @param y y_pos to start draw the numbers
 * @param digit_len max lengh of the score
 */
void (drawNumbers)(int value,int x, int y, int digit_len);
/**
 * @brief draws the bar of score on the screen
 */
void (drawScore)();
/**
 * @brief moves the body of the snake
 */
void (moveSegments)();
/**
 * @brief increments the size of the snake
 * @param tailX x_pos of the tail of the snake
 * @param tailY y_pos of the tail of the snake
 */
void (incrementSnake)(int tailX, int tailY);
/**
 * @brief draws an apple on the screen
 */
void (drawApple)();
/**
 * @brief detects if exists an apple on the screen, if doesn't, draws a new one
 */
void (updateApple)();
/**
 * @brief creates a new enemy at a random position
 */
void (spawnEnemy)();
/**
 * @brief moves the enemy based on the head of the snake
 */
void (moveEnemy)();
/**
 * @brief erases the enemy after this touches the snake
 */
void (eraseEnemyTrail)();
/**
 * @brief takes off one segment of the snake body
 */
void (damageSnake)();
/**
 * @brief draws a new enemy
 */
void (drawEnemy)();
/**
 * @brief erases the enemy when it was killed by the user
 */
void (killEnemy)();


