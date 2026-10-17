#ifndef __GRAPHICS_H
#define __GRAPHICS_H

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

char *getGraphicsBuffer(void);
void bufferToVideoMem(void);
void clearBuffer(void);

 /** @} end of video_gr */

int drawSquare(unsigned short x, unsigned short y, unsigned short size, unsigned short color);

void drawLine(unsigned xi, unsigned yi, unsigned xf, unsigned yf, char color);


 /**
 * @brief funtion to get vertical resolution
 * @return V_RES;
 */
int getVerResolution();

 /**
 * @brief funtion to get horizontal resolution
 * @return H_RES;
 */
int getHorResolution();

 /**
 * @brief funtion that initizalizes double buffer
 */
void initDoubleBuffer();


#endif /* __GRAPHICS_H */
