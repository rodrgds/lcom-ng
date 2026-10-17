#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

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
 * @brief Sets Pixel to Color
 * @param x The X Coord
 * @param y The Y Coord
 * @param color The Color
 * @param ptr Pointer To Vram
 */
void setColorPixel(int x, int y, int color, short * ptr);

/**
 * @brief Check if it is a Valid Coord
 * @param x The X Coord
 * @param y The Y Coord
 * @return 1 upon success, 0 upon failure
 */
int validCoord(int x, int y);

/**
 * @brief Copy the buffer to the video memory (Double Buffering).
 */
void flipDisplay();

/**
 * @brief Gets Horizontal Resolution of the Screen
 *
 * @return The Horizontal Resolution of the Screen
 */
short getHorResolution();

/**
 * @brief Gets Vertical Resolution of the Screen
 *
 * @return The Vertical Resolution of the Screen
 */
short getVerResolution();

/**
 * @brief Gets The buffer in which the Bitmaps are written
 *
 * @return The Buffer
 */
char* getGraphicsBuffer();

/** @} end of video_gr */

#endif /* __VIDEO_GR_H */
