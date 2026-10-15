#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <stdbool.h>
#include <stdint.h>
#include <lcom/lcf.h>


vbe_mode_info_t vmi_p;
/**
 * @brief maps vram
 * 
 */
void vramMap();
/**
 * @brief Set the graphics mode to the one desired
 * 
 * @param mode Mode to be user on graphics card
 * @return int 0 if succeded and 1 otherwise
 */
int setMode(uint16_t mode);

/**
 * @brief places de xpm given in the position given on the screen
 * 
 * @param img 
 * @param map 
 * @param x 
 * @param y 
 */
void draw_xpm(xpm_image_t img, uint8_t *map, int x, int y);

/**
 * @brief places de given in the position given on the screen on the olher buffer
 * 
 * @param img 
 * @param map 
 * @param x 
 * @param y 
 */
void draw_xpm_video_mem(xpm_image_t img, uint8_t *map, int x, int y);

/**
 * @brief Erase de xpm by replacing it with the background color in that place
 * 
 * @param img 
 * @param x 
 * @param y 
 * @param background 
 */
void erase_xpm(xpm_image_t img, int x, int y, xpm_image_t background);

/**
 * @brief copys to video mem the olher buffer
 * 
 */
void copy_buffer_to_mem();

/** @brief Process (virtual) address to which VRAM is mapped */
char *video_mem;	
/** @brief Copy of video_mem */	
char *video_mem_buf;

/** @brief Horizontal resolution in pixels */
int h_res;	       
/** @brief Vertical resolution in pixels  */
int v_res;	       
/** @brief Number of VRAM bits per pixel  */
unsigned bits_per_pixel; 
/** @brief Number of VRAM bytes per pixel  */
unsigned bytes_per_pixel;


#endif /* _GRAPHICS_H_ */
