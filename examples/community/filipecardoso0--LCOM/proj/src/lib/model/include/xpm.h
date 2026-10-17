#ifndef _XPM_H_
#define _XPM_H_

#include <lcom/lcf.h>

#include "../../../drivers/graphic/include/graphic.h"

struct xpm {
    uint16_t x, y, width, height;
    uint8_t* map;
};

typedef struct xpm xpm_t;

/**
 * @brief Creates a new xpm object in memory
 * 
 * @param xpm map of the shape to be created
 * @param x x coordinates of the xpm object
 * @param y y coordinates of the xpm object
 * 
 * @return pointer the newly created object
 */
xpm_t* xpm_new(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Deletes a xpm object from memory
 * 
 * @param xpm pointer to object that is to be deleted
 */
void xpm_delete(xpm_t* xpm);

/**
 * @brief Sets new coordinates for a xpm object
 * 
 * @param xpm pointer to xpm object that is to be changed
 * @param x new x coordiantes of the xpm object
 * @param y new y coordinates of the xpm object
 */
void xpm_set_position(xpm_t* xpm, uint16_t x, uint16_t y);

/**
 * @brief Gets the width of a xpm object
 * 
 * @param xpm the xpm to get the width from
 * 
 * @return width of the xpm object
 */
uint16_t xpm_get_width(xpm_t* xpm);

/**
 * @brief Gets the height of a xpm object
 * 
 * @param xpm the xpm to get the height from
 * 
 * @return height of the xpm object
 */
uint16_t xpm_get_height(xpm_t* xpm);



#endif
