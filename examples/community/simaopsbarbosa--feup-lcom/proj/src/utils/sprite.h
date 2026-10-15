/**
 * @file sprite.h
 * @brief Sprite structure and utility functions for XPM image handling.
 */

 #ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_
#include <lcom/lcf.h>

/**
 * @struct Sprite
 * @brief Represents a graphical sprite.
 * 
 * Contains the sprite image and a pointer to its pixel data.
 */
struct Sprite {
    /**
     * @brief The XPM image of the sprite.
     */
    xpm_image_t img;

    /**
     * @brief Pointer to the sprite's pixel data.
     */
    uint32_t *data;
};

#endif /* _LCOM_SPRITE_H_ */

