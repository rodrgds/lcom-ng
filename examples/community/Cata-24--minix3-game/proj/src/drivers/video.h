/**
 * @file video.h
 * @brief Function declarations used for the video modes, pixels and sprites.
 */


#ifndef _PROJ_VIDEO_
#define _PROJ_VIDEO_

#include <lcom/lcf.h>
#include "../helper/structs.h"


/**
 * @brief Sets the video mode.
 * 
 * @param mode The desired VBE Mode.
 * @return int 0 on success, 1 on failure.
 */ 
int (setBIOSMode)(uint16_t mode);


/**
 * @brief Sets a VBE command.
 * 
 * @param command The desired command to call.
 * @param mode The video mode for the command.
 * @return int 0 on success, 1 on failure.
 */
int (setVBECommand)(uint16_t command, uint16_t mode);


/**
 * @brief Maps the video memory into the process address space.
 * 
 * Uses a pointer and mode information to map VRAM.
 * Allocates a temporary buffer for double buffering.
 * 
 * @param mode The video mode.
 * @return int 0 on success, 1 on failure.
 */
int (get_map_mem)(uint16_t mode);


/**
 * @brief Copies the temporary buffer to VRAM.
 * 
 * Implements double buffering to prevent visual artifacts.
 * 
 * @return int 0 on success, 1 on failure.
 */
int (buffer_flip)();


/**
 * @brief Clears the temporary buffer.
 * 
 * All pixels set to 0.
 * 
 * @return int 0 on success, 1 on failure.
 */
int (clear_temp_buf)();


/**
 * @brief Draws a pixel on the temporary buffer at coordinates x, y  with the given color.
 * 
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Color to paint the pixel.
 * @return int 0 on success, 1 on failure.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);


/**
 * @brief Draws a sprite.
 * 
 * Uses reverse to transform Sprites to their reversed direction.
 * 
 * @param sprite Pointer to the sprite struct.
 * @param reversed If true, draw sprite mirrored.
 * @return int 0 on success, 1 on failure.
 */
int (drawSprite)(struct Sprite *sprite, bool reversed);


/**
 * @brief Loads a sprite from an XPM map.
 * 
 * Allocates memory for the sprite and initializes it.
 * 
 * @param container Pointer to the sprite pointer that is going to be loaded.
 * @param sprite The XPM map to load.
 * @return int 0 on success, 1 on failure.
 */
int (loadSprite)(struct Sprite **container, xpm_map_t sprite);

#endif // _PROJ_VIDEO_ 
