#include <stdbool.h>
#include <stdint.h>
#include <lcom/lcf.h>
#include "graphics.h"
/**
 * @brief struct of an snake bodypart 
 * 
 */
typedef struct{
    int x;
    int y;
    int oldx;
    int oldy;
    int changex;
    int changey;
    char * direction;
    char * prevDirection;
    xpm_image_t img;
    uint8_t *map;
    xpm_image_t imgUp;
    uint8_t *mapUp;
    xpm_image_t imgDown;
    uint8_t *mapDown;
    xpm_image_t imgLeft;
    uint8_t *mapLeft;
    xpm_image_t imgRight;
    uint8_t *mapRight;
}snake_part;

/** @brief array of that represents the snake */
snake_part snakeBody[198];
/** @brief tail of the snake*/
snake_part snakeTail;
/** @brief number of body parts of the snake*/
int numOfBodyParts;
/** @brief if true, snake starts moving*/
bool start;
/** @brief lives of the snake*/
double lives;
/** @brief number of apples eaten*/
int numOfApplesEaten;
/** @brief number of apples needed to win half live*/
int gainLive;


/**
 * @brief initiates all aspects of the snake
 *
 */
void init_snake();

/**
 * @brief deals with the movement and collisions of a snake
 * 
 * @param speed of the movement
 * @return int 1 if the snake died
 */
int movement(int16_t speed);

/**
 * @brief draws snake
 * 
 */
void drawSnakeBody();

/**
 * @brief adds a body part to the snake
 * 
 */
void addBodyPart();

/**
 * @brief moves the body of the snake to the next position
 * 
 */
void moveBodyParts();

/**
 * @brief takes n lives form the snake
 * 
 * @param n number of lives to be taken away
 * @return int 1 if the snake has no lives left
 */
int takelive(double n);

/**
 * @brief checks if their is any bodyPart in that position
 * 
 * @param x column of the position
 * @param y line of the position
 * @return int 1 if the snake is in that position
 */
int isSnake(int x, int y);

/**
 * @brief checks if there has been a collision with the snake and a apple
 * 
 * @return int 1 if the snake has died
 */
int collisionWithApple();

/**
 * @brief checks the snake has collided against itselph 
 * 
 * @return int 1 if the collision happend
 */
int collisionWithItSelf();

/**
 * @brief draws an apple at a random position in a periodic time
 * 
 */
void periodicApples();
