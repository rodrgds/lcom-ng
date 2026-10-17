#ifndef _LCOM_GRAPHIC_H_
#define _LCOM_GRAPHIC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "sprite.h"

#define BLACK 0x00
#define TRANSPARENT 0x999999

/**
     * @brief Mapping of video RAM (VRAM) into the process address space.
     * 
     * @param mode specifies the current mode (0x115 or 0x14C)
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (map_vram)(uint16_t mode);
/**
     * @brief Make a real-mode BIOS [call] on behalf of a user-space device driver. 
     * This temporarily switches from 32-bit protected mode to 16-bit real-mode to access the BIOS calls.
     * 
     * @param mode specifies the current mode (0x115 or 0x14C)
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (set_mode)(uint16_t mode);
/**
     * @brief Draws a coloured pixel on the screen at the specified position.
     * 
     * @param x horizontal pixel resolution
     * @param y vertical pixel resolution
     * @param color to be displayed in the pixel position
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
     * @brief Draws a given xpm Sprite on the screen at the specified position  
     * 
     * @param x horizontal resolution
     * @param y vertical resolution
     * @param xpm xpm_map_t to be displayed
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int drawSpriteXPM(Sprite *sprite, int x, int y);
/**
     * @brief Erases the screen and draws a new page according to the game current state 
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (graphic_update)();
/**
     * @brief Allocates memory and set the double buffer into the game mode
     * 
     * @param mode specifies the current mode (0x115 or 0x14C)
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int alloc_double_buffer(uint16_t mode);


#endif
