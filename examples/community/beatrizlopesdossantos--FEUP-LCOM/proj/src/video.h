#ifndef VIDEO_H
#define VIDEO_H

#define VBE_MODE        0x14C /**< @brief Value of the VBE mode */
#define PB2BASE(x)      (((x) >> 4) & 0x0F000) /**< @brief Defined in liblm */ 
#define PB2OFF(x)       ((x) & (0x0FFFF)) /**< @brief Defined in liblm */ 

/**
 * @defgroup video
 * @{ 
 * @brief Functions of the Video Card
 */

/**
* @brief Initializes the video module in graphics mode.
*
* @brief Uses the VBE INT 0x10 interface to set the desired graphics mode using a linear frame buffer, maps VRAM to the process' address space and initializes static global variables with the resolution of the screen, and the color depth (i.e the no. of bits per pixel).
* @param mode 16-bit VBE mode to set
* @return Virtual address VRAM was mapped to. NULL, upon failure.
*/
void *(vg_init)(uint16_t mode);

/**
* @brief Updates the memory used in video_mem
*/
void refresh();

/**
* @brief Clears the second buffer.
*/
void clear_sec_buff();

/**
* @brief Colors the pixel with the position indicated.
* @param x horizontal position
* @param y vertical position
* @param color color wanted to be drawn
*/
void vg_color_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
* @brief Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address.
*
* @brief Initializes packed vbe_mode__info_t structure passed as an address with the VBE information on the input mode, by calling VBE function 0x01, Return VBE Mode Information, and copying the ModeInfoBlock struct returned by that function.
* @param mode mode whose information should be returned
* @param vmi_p address of vbe_mode_info_t structure to be initialized
* @return 0 on success, non-zero otherwise
*/
int (vbe_get_mode_info_mine)(uint16_t mode, vbe_mode_info_t* vmi_p);

/** @} end of video */

#endif //VIDEO_H
