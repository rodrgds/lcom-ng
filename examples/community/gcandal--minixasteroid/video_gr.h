#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#define H_RES             	1024
#define V_RES				768

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/*
 * @brief Copies one array to the other
 *
 * Copies the information in the secondary buffer to the VRAM
 */
void vg_buffertomem();
/*
 * @brief Copies one array to the other
 *
 * Copies the information from the VRAM to the secondary buffer
 */
void vg_memtobuffer();
/*
* @brief Gets the color from the input pixel
*
* Gets the color from the pixel at the specified position to the input value,
*  by reading from the corresponding location in VRAM
*
* @param x horizontal coordinate, starts at 0 (leftmost pixel)
* @param y vertical coordinate, starts at 0 (top pixel)
* @return Color of the input pixel
*
*/
char vg_get_pixel_buffer(unsigned long x, unsigned long y);

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
void *vg_init(unsigned long mode);

/**
 * @brief Fills the screen with the input color
 * 
 * @param color color to fill the screen with
 * @return 0 on success, non-zero upon failure
 */
int vg_fill(unsigned long color);
/**
 * @brief Fills the secondary buffer with the input color
 *
 * @param color color to fill the secondary buffer with
 * @return 0 on success, non-zero upon failure
 */
int vg_fill_buffer(unsigned long color);

/**
 * @brief Sets input pixel with input color
 * 
 * Sets the color of the pixel at the specified position to the input value, 
 *  by writing to the corresponding location in VRAM
 * 
 * @param x horizontal coordinate, starts at 0 (leftmost pixel)
 * @param y vertical coordinate, starts at 0 (top pixel)
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color);
/**
 * @brief Sets input pixel with input color
 *
 * Sets the color of the pixel at the specified position to the input value,
 *  by writing to the corresponding location in the secondary buffer
 *
 * @param x horizontal coordinate, starts at 0 (leftmost pixel)
 * @param y vertical coordinate, starts at 0 (top pixel)
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int vg_set_pixel_buffer(unsigned long x, unsigned long y, unsigned long color);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
