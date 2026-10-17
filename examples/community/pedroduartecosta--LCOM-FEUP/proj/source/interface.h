#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "MainMenuState.h"

/** @defgroup Button Button
 * @{
 * functions regarding the interface
 */

/**
 * @brief exits the game
 *
 * @return int saying if sucessuful
 */
int exitGame();

/**
 * @brief exits the game
 *
 *@param intarface mode
 *
 */

void *initGame(unsigned short mode);

/**
 * @brief sets pixel color
 *
 *@param x coordinate
 *@param y coordinate
 *@param color
 *
 */

void fill_pixel(unsigned short x, unsigned short y, unsigned short color);

/**
 * @brief returns vertical resolution
 *
 * @return vertical resolution
 */
unsigned getVerResolution();

/**
 * @brief returns horizontal resolution
 *
 * @return horizontal resolution
 */

unsigned getHorResolution();

/**
 * @brief gets graphics buffer
 *
 * @return pointer to graphics buffer
 */

char* getGraphicsBuffer();

/**
 * @brief gets temporary graphics buffer
 *
 * @return pointer to temporary graphics buffer
 */

char* getGraphicsBufferTmp();

/**
 * @brief gets size of the vram
 *
 * @return size of the vram
 */

unsigned getVRAMSize();

/**
 * @brief gets bytes per pixel
 *
 * @return number of bytes per pixel
 */

unsigned getBytesPerPixel();

/**
 * @brief flips display
 *
 * @param vm
 *
 */

void flipDisplay(char* vm);

/**
 * @brief reads xmp
 *
 * @param xmp map
 * @param xpm widht
 * @param xpm heith
 *
 * @return pointer to xpm
 */
char *read_xpm(char *map[], int *wd, int *ht);

/**
 * @brief prints xpm
 *
 * @param x coordinate
 * @param y coordinate
 * @param xpm to be printed
 *
 * @return int
 */

int print_xpm(unsigned short xi, unsigned short yi, char *xpm[]);

/**
 * @brief copies from the temporary buffer to buffer
 *
 */

void refresh();

/**@}*/

#endif
