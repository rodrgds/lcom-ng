#ifndef VIDEO_H
#define VIDEO_H
#include <lcom/lcf.h>



#define IGNORED 0xFF00F0


/**
 * @brief struct with details of the mode we're using 
 * 
 */
extern vbe_mode_info_t vmi_p;

/**
 * @brief second buffer
 * 
 */
extern char* video_buff;

/**
 * @brief Set the vbe mode object
 * 
 * @param mode desired mode
 * @return int 0 in case it goes successfully,1 otherwise 
 */
int (set_vbe_mod)(uint16_t mode);

/**
 * @brief sets a color of a pixel
 * 
 * @param x horizontal position of the pixel
 * @param y vertical position of the pixel
 * @param color the desired color for the pixel
 */
void (pixel_set_color)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief parses the content of the second buffer to the main buffer
 * 
 */
void (double_buff)();

/**
 * @brief clears the screen
 * 
 */
void (vg_clear_screen)();

/**
 * @brief loads an image and displays it 
 * 
 * @param xpm the image we want to load
 * @param x horizontal position of the image
 * @param y vertical position of the image
 * @return int 0 in case it goes sucessfully, 1 otherwise
 */
int (vg_read_xpm)(xpm_map_t xpm,uint16_t x, uint16_t y);


#endif
