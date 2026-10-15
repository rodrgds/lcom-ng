#ifndef __OBJECTS_H
#define __OBJECTS_H


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <lcom/lcf.h>



/**
 * @brief struct that represents an object with their xpm image and position
 * 
 */
typedef struct{
    xpm_image_t img;
    uint8_t *map;
}object;

object background;

void drawBackground();


#endif /* __OBJECTS_H */
