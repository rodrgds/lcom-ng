#ifndef SPRITE_H
#define SPRITE_H
#include <lcom/lcf.h>
#include <math.h>



/** @defgroup sprite 
 * @{
 *
 * Functions related to sprite 
 */


typedef struct{
  xpm_image_t* image;
  uint8_t imageIndex;
  uint8_t* arr;
  int x, y; // 1000mx = 1x
  double rotation, realx, realy;
  double speedX, speedY;
  //int mspeedX, mspeedY; // 1000mspeed = 1 speed
  uint8_t isMovementUnique; // boolean
  uint8_t ignoresTime; // boolean 1 if the 'time mechanic' doesn't apply to this sprite 
} Sprite;


/**
 * @brief Create a sprite with the given image and at x and y
 * 
 * @param image 
 * @param x 
 * @param y 
 * @return Sprite* 
 */
Sprite * createSprite(xpm_image_t *image, int x, int y);

/**
 * @brief Get center of a sprite via the arguments x and y
 * 
 * @param sprite 
 * @param x 
 * @param y 
 */
void getCenter(Sprite *sprite, int *x, int *y);

/**
 * @brief Check colision between 2 sprites, only checks the 'rectangles' of the sprites
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int checkColision(Sprite *a, Sprite *b);

/*
 * Given the position of a Point Or value of a vector, gives it's angle.
 *
 */
double getAngle(int x, int y);

/**
 * @brief Get rotation of the sprite when facing the x and y coordinates
 * 
 * @param source 
 * @param x 
 * @param y 
 * @return double 
 */
double getRotation(Sprite *source, unsigned x, unsigned y);

/**
 * @brief Transforms the oldx and oldy
 * 
 * @param sprite 
 * @param oldx 
 * @param oldy 
 * @param newx 
 * @param newy 
 */
void getRotationCoordinate(Sprite *sprite, int oldx, int oldy , int *newx, int *newy);

/**
 * @brief Set the sprite speed with the value of speed and direction of vectorX,vectorY
 * 
 * @param sprite 
 * @param speed 
 * @param vectorX 
 * @param vectorY 
 */
void setSpeed(Sprite *sprite, int speed, int vectorX, int vectorY);

/**
 * @brief Get the Color of a pixel at (x, y) of the sprite
 * 
 * @param sprite 
 * @param x 
 * @param y 
 * @return uint32_t 
 */
uint32_t getColor(Sprite *sprite, int x, int y);

/**
 * @brief Forces the sprite to be inside of the screen
 * 
 * @param x_res 
 * @param y_res 
 * @param sprite 
 */
void keepInsideScreen(int x_res, int y_res, Sprite *sprite);
#endif
