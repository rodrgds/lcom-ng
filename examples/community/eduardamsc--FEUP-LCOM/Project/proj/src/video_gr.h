#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

#define HRES	1024
#define VRES	768

/**
 * @brief Changes pixel with coordinates (x,y) to desired color
 *
 * @param x Column coordinate
 * @param y Line coordinate
 * @param color 16bit hexadecimal code which defines a color
 */
void paint_pixel(unsigned short x, unsigned short y, unsigned long color);
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
 * @brief Checks if coordinates are inside screen
 *
 * @param x Column coordinate
 * @param y Line coordinate
 * @return 0 upon success, non-zero upon failure
 */
int check_coordinates(unsigned short x, unsigned short y);

/**
 * @brief Gets buffer
 * @return Buffer
 */
char* getGraphicsBuffer();

/**
 * @brief Copies auxiliar buffer to main bugger
 */
void flipBuffer();

/**
 * @brief Gets process (virtual) address to which VRAM is mapped
 * @return Process (virtual) address to which VRAM is mapped
 */
char* getVideoMem();

/**
 * @brief Gets vertical screen resolution in pixels
 * @return Vertical screen resolution in pixels
 */
unsigned getVerResolution();

/**
 * @brief Gets horizontal screen resolution in pixels
 * @return Horizontal screen resolution in pixels
 */
unsigned getHorResolution();

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
