#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

// Returns Horizontal resolution
unsigned int getHorResolution();

// Returns Vertical resolution
unsigned int getVerResolution();

// Returns graphics buffer pointer
short * getGraphicsBuffer();

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


//Draws pixel of given color in a given x, y position
int video_draw_pixel(unsigned short x, unsigned short y, unsigned long color);

//Read xpm
char *read_xpm(char *map[], int *wd, int *ht);

//paints all pixels black
int clearScreen();

//returns temporary buffer pointer (for double buffering)
short* getTmpBuffer();

//returns vram size
unsigned int getVramSize();

 /** @} end of video_gr */

#endif /* __VIDEO_GR_H */
