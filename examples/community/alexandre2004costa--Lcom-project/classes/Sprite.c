#include "Sprite.h"

/** @defgroup Sprite Sprite
* @{
 */

/**
 * @brief Creates a sprite (with position, dimension and a map) from a xpm and a position.
 * @param xpm 
 * @param x Position on the x axis.
 * @param y Position on the y axis.
 * @return The created sprite.
 */
Sprite *create_sprite(xpm_map_t xpm, int x, int y) {
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    xpm_image_t img;
    if(sp == NULL) return NULL;
    sp->map = (uint32_t *)xpm_load(xpm, XPM_8_8_8_8, &img);
    if(sp->map == NULL) {
        free(sp);
        return NULL;
    }
    sp->width = img.width; 
    sp->height= img.height;
    sp->x = x;
    sp->y = y;
    return sp;
}

/**
 * @brief Draws a sprite using video graphics coloring.
 * @param sprite
 * @see pixel_color() for the coloring of a pixel.
 */
void drawSprite(Sprite *sprite){ 
   int c = 0;
   for (int i = 0 ; i < sprite->height ; i++) {
        for (int j = 0 ; j < sprite->width ; j++) {
            pixel_color(sprite->x + j, sprite->y + i, *(sprite->map + c));
            c++;
        }
    }
}


/**
 * @brief In case you need to change the xpm (appearance) of a certain sprite.
 * @param sp
 * @param xpm New xpm.
 */
void changeSprite(Sprite *sp, xpm_map_t xpm){
    xpm_image_t img;
    sp->map = (uint32_t *)xpm_load(xpm, XPM_8_8_8_8, &img);
    if(sp->map == NULL) {
        free(sp);
        return;
    }
    sp->width = img.width; 
    sp->height= img.height;
}


/**
 * @brief In a xpm map where there are multiple images for individual sprites, this function retrieves only the desired one.
 * @param index Index of the wanted image.
 * @param wXpm Number of images in each row.
 * @param hXpm Number of images in each column.
 * @param sp
 * @see pixel_color() for the coloring of a pixel.
 */
void drawMultipleSprite(int index, int wXpm, int hXpm, Sprite *sp){
     int width = sp->width / wXpm;
     int height = sp->height / hXpm;
     int totalPix = wXpm * hXpm;
     if (index < 0 || index >= totalPix) {
        return;
    }
    int spriteX = (index % wXpm) * width;
    int spriteY = (index / wXpm) * height;
    for (int i = 0 ; i < height ; i++) {
        for (int j = 0 ; j < width ; j++) {
            int mapIndex = (spriteY + i) * sp->width + (spriteX + j);
            pixel_color(sp->x + j, sp->y + i, sp->map[mapIndex]);
        }
    }
}


/**@}*/




