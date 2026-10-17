#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <lcom/lcf.h>
#include "sprite.h"
#include "enemy.h"
#include "player.h"
#include "gun.h"
#include "letters.h"


/** @defgroup graphics
 * @{
 *
 * Functions related to the graphics
 */

//Standard BIOS Services
#define BIOS_VIDEO_CARD 0x10
#define BIOS_PC_CONF  0x11
#define BIOS_MEMORY_CONF  0x12
#define BIOS_KEYBOARD 0x16



//VBE function call results
#define VBE_CALL_SUCCESS 0x00
#define VBE_ERROR_CALL 0x01
#define VBE_ERROR_NOTSUPPORTED 0x02 // Function is not supported in current HW() configuration
#define VBE_ERROR_INVALID_VM 0x03 //Function is Invalid in current Video Mode


//Graphics Mode                   Color Model =        | Bits per pixel(R:G:B)
#define GM_1024x768   0x105 //    Indexed              | 8
#define GM_640x480    0x110 //    Direct color         | 15((1:)5:5:5)
#define GM_800x600    0x115 //    Direct color         | 24(8:8:8)
#define GM_1280x1024  0x11A //    Direct color         | 16(5:6:5)
#define GM_1152x864   0x14C //    Direct color         | 32((8:)8:8:8)

#define VBE_CALL 0x4F02 // Use with a function [VBE_CALL < 2 | ST ]

//Functions
#define SET_VBE_MODE 0x02 // BX argument [0, 8] -> mode number | [9- 13] -> 0 
//             [14] = (0 - Use windowed frame buffer model; 1 - User linear/flat fame buffer model) 
//             [15] -> (0 - Clear display memory; 1 - don't clear display memory)

#define VBE_FUNCTION_SET_VBE 0x4F02
#define VBE_FUNCTION_SETGET_DISPLAY_START 0x4F07




/**
 * @brief Activate graphics mode in the given mode
 * 
 * @param mode 
 * @return int 
 */
int activateGraphicsMode(uint16_t mode);

/**
 * @brief Sets to text Mode
 * 
 * @return int 
 */
int activateTextMode();

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draw a pixel at x and y, with the given color
 * 
 * @param x 
 * @param y 
 * @param color 
 * @return int 
 */
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draw pixel, only draws if it's white. Returns the color of the pixel before drawing. 
 * 
 * @param x 
 * @param y 
 * @param color 
 * @return uint32_t 
 */
uint32_t draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draw pixmap
 * 
 * @param xpm 
 * @param x 
 * @param y 
 * @return int 
 */
int draw_pm(xpm_map_t xpm, uint16_t x, uint16_t y);


/**
 * @brief Draws the sprite with the given sprite.rotation
 * 
 * @param sprite 
 * @return int 
 */
int drawSprite(Sprite *sprite);

/**
 * @brief Draw Player and its gun
 * 
 * @param player 
 * @return int 
 */
int drawPlayer(Player *player);

/**
 * @brief Draw enemy and its gun
 * 
 * @param enemy 
 * @return int 
 */
int drawEnemy(Enemy *enemy);

/**
 * @brief Draw Gun
 * 
 * @param gun 
 * @return int 
 */
int drawGun(Gun *gun);

/**
 * @brief Draw bullet
 * 
 * @param bullet 
 * @return int 
 */
int drawBullet(Bullet * bullet);

/**
 * @brief Draw a text, accepts all letters, numbers and '-'
 * 
 * @param str 
 * @param x 
 * @param y 
 * @return int 
 */
int drawString(char * str, int x, int y);

/**
 * @brief Returns 1 if point is out of bounds, with a given tolerance, 0 otherwise
 * 
 * @param x 
 * @param y 
 * @param tolerance 
 * @return int 
 */
int is_out_of_bounds(int x, int y, int tolerance);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 *  Copy double buffer to the video memory or uses pageFlipping if usePageFlipping is true
 */
void applyChanges();

/**
 * @brief Returns the resolution of the current mode via arguments 
 * 
 * @param xres 
 * @param yres 
 */
void getResolution(int *xres, int *yres);

#endif
