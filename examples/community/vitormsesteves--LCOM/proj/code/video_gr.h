#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H


#include "rectangle.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

#define GRAPHIC_MODE 0x114
#define BIOS_VIDEO 0x10
#define VBE 0x4F
#define VIDEO 0x00
#define SET_VBE 0x02
#define SET_TEXT 0x03
#define LINEAR_BIT 14

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */
static char *buffer;		/* Process address to auxiliary buffer */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel; /* Number of VRAM bytes per pixel */
static unsigned int vram_size; /* Size of VRAM bits per pixel */

#define SWAP(X,Y) { \
       int temp = X ; \
       X = Y ; \
       Y = temp ; \
    } /**< @brief Swap values */



/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);


/**
* @brief Draw a pixel
*
* @param x X pixel cord
* @param y Y pixel cord
* @param color Pixel color
* @param buf Buffer to draw
*/
void draw_pixel(unsigned int x, unsigned int y, int color, char* buf);

/**
* @brief Draw a Rectangle
*
* @param xi X pixel cord
* @param yi Y pixel cord
* @param xf X pixel cord
* @param yf Y pixel cord
* @param color Rectangle color
* @param buf Buffer to draw
* @return Validation
*/
int draw_rectangle(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, int color, char* buf);

/**
* @brief Draw a circle
*
* @param x0 X center cord
* @param y0 Y center cord
* @param radius X pixel final cord
* @param color rectangleCircle color
* @param buf Buffer to draw
*/
void draw_circle(int x0, int y0, int radius, int color, char* buf);

/**
* @brief Cover a screen except a circle
*
* @param x0 X center cord
* @param y0 Y center cord
* @param radius X pixel final cord
* @param color rectangleCircle color
* @param buf Buffer to draw
*/
void see_circle(int x0, int y0, int radius, int color, char* buf);

/**
* @brief Get a graphics buffer
*
* @return Graphics buffer
*/
char* getGraphicsBuffer();

/**
* @brief Change Graphics buffer to Video Mem
*
*/
void flipScreen();


/**
* @brief Get horizontal resolution
*
* @return Horizontal resolution
*/
unsigned getH_res();
/**
* @brief Get vertical resolution
*
* @return Vertical resolution
*/
unsigned getV_res();

/**
* @brief Creates a specific color
*
* @param r fraction of red
* @param g fraction of green
* @param b fraction of blue
* @return Result color
*/
int rgb(unsigned char r, unsigned char g, unsigned char b);

#endif /* __VIDEO_GR_H */
