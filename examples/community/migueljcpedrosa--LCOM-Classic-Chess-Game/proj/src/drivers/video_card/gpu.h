/**
 * @file gpu
 * @brief This file has the implementation of the functions related to the video graphics.
 */

#ifndef _GPU_H_
#define _GPU_H_

#include <lcom/lcf.h>

extern unsigned int h_res;	     
extern unsigned int v_res;	

/**
 * @brief Gets the video mode information.
 * 
 * This function returns the infomation about the video mode provided, in the form of a vbe_mode_info_t struct.
 * It uses the 0x10 interrupt number for video services, and sets AH register to the write teletype 
 * function (0x4F) and AL register to specify the function to get the VBE mode information (0x01).
 * 
 * @param mode The desired video mode.
 * @param vbe_info Pointer to the vbe_mode_info_t struct to be filled.
 * 
 * @return Returns 0 upon success and 1 upon failure.
 */
int (get_vbe_mode_info)(uint16_t mode, vbe_mode_info_t *vbe_info);

/**
 * @brief Sets the video display mode.
 *
 * The function sets the video mode to the one indicated by the parameter 'mode', 
 * and configures 'index_mode' as true if the mode is MODE1, and false otherwise. 
 * It uses the 0x10 interrupt number for video services, and sets AH register to 
 * the write teletype function (0x4F) and AL register to specify the function 
 * to set the VBE mode (0x02). 
 * The function will fail if the mode change was not supported or if any error occurred.
 * 
 * @param mode The desired video display mode.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int (set_mode)(uint16_t mode);

/**
 * @brief Maps VBE mode information into memory.
 * 
 * @param vmi_p Pointer to VBE mode information structure.
 * 
 * @return Returns 0 upon success and 1 upon failure.
 */
int (map_info)(vbe_mode_info_t* vmi_p);

/**
 * @brief Cleans the screen.
 * 
 * @return Returns 0 upon success.
 */
int (clean_screen)();

/**
 * @brief Draws a rectangle in the buffer.
 * 
 * @param x X-coordinate for the top-left corner of the rectangle.
 * @param y Y-coordinate for the top-left corner of the rectangle.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color of the rectangle.
 * 
 * @return Returns 0 upon success and 1 upon failure.
 */
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Copies the current buffer content to the screenshot buffer.
 * 
 * @return Returns 0 upon success.
 */
int (copy_buffer_to_screenshot)();

/**
 * @brief Draws the screenshot content into the buffer, using given coordinates and size.
 * 
 * @param x X-coordinate for the top-left corner of the screenshot content.
 * @param y Y-coordinate for the top-left corner of the screenshot content.
 * @param width Width of the screenshot content.
 * @param height Height of the screenshot content.
 * 
 * @return Returns 0 upon success.
 */
int (draw_screenshot_to_buffer)(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

/**
 * @brief Copies the buffer content to the video memory.
 * 
 * @return Returns 0 upon success.
 */
int (copy_buffer_to_video_mem)();

/**
 * @brief Draws a pattern of rectangles on the screen.
 * 
 * @param no_rectangles Number of rectangles in each row and column of the pattern.
 * @param first Color of the first rectangle.
 * @param step Step to calculate the color of the subsequent rectangles.
 * 
 * @return Returns 0 upon success and 1 upon failure.
 */
int (draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Extracts the red value from a given color.
 * 
 * @param first The color from which the red value is extracted.
 * 
 * @return The red value.
 */
uint8_t R(uint32_t first);

/**
 * @brief Extracts the green value from a given color.
 * 
 * @param first The color from which the green value is extracted.
 * 
 * @return The green value.
 */
uint8_t G(uint32_t first);

/**
 * @brief Extracts the blue value from a given color.
 * 
 * @param first The color from which the blue value is extracted.
 * 
 * @return The blue value.
 */
uint8_t B(uint32_t first);

/**
 * @brief Determines the image type based on the provided mode.
 * 
 * @param type Pointer to store the resultant image type.
 * @param mode Mode that determines the image type.
 * 
 * @return Returns 0 upon success and 1 upon failure.
 */
int (get_xpm_image_type)(enum xpm_image_type* type, uint16_t mode);

/**
 * @brief Loads an XPM image.
 * 
 * @param xpm XPM image to be loaded.
 * @param type XPM image type.
 * @param sprite Pointer to store the loaded XPM image.
 * @param img_addr Pointer to store the address of the loaded image.
 * 
 * @return Returns 0 upon success and 1 upon failure.
 */
int (load_xpm)(xpm_map_t xpm, enum xpm_image_type type, xpm_image_t* sprite,  uint8_t** img_addr);

/**
 * @brief Draws an XPM image on the screen.
 * 
 * @param sprite XPM image to be drawn.
 * @param img_addr Address of the image to be drawn.
 * @param x X-coordinate for the top-left corner of the image.
 * @param y Y-coordinate for the top-left corner of the image.
 * 
 * @return Returns 0 upon success.
 */
int (draw_xpm)(xpm_image_t sprite, uint8_t* img_addr, uint16_t x, uint16_t y);

/**
 * @brief Erases an XPM image from the screen.
 * 
 * @param xpm XPM image to be erased.
 * @param type XPM image type.
 * @param x X-coordinate for the top-left corner of the image.
 * @param y Y-coordinate for the top-left corner of the image.
 * 
 * @return Returns 0 upon success and 1 upon failure.
 */
int (erase_xpm)(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y);

/**
 * @brief Moves a sprite on the screen.
 * 
 * @param xi Pointer to the initial X-coordinate of the sprite.
 * @param yi Pointer to the initial Y-coordinate of the sprite.
 * @param xf Final X-coordinate of the sprite.
 * @param yf Final Y-coordinate of the sprite.
 * @param speed Speed at which the sprite moves.
 * @param frame_count Number of frames elapsed since the beginning of the game.
 * 
 * @return Returns true if the sprite has reached its destination, and false otherwise.
 */
bool (move_sprite)( uint16_t* xi, uint16_t* yi, uint16_t xf, uint16_t yf, int16_t speed, uint32_t frame_count);

/**
 * @brief Draws a letter from an XPM image on the screen.
 * 
 * @param sprite XPM image containing the alphabet.
 * @param img_addr Address of the image.
 * @param x X-coordinate for the top-left corner of the letter.
 * @param y Y-coordinate for the top-left corner of the letter.
 * @param letter Letter to be drawn.
 * 
 * @return Returns 0 upon success.
 */
int (draw_xpm_letter)(xpm_image_t sprite, uint8_t* img_addr, uint16_t x, uint16_t y, char letter);

/**
 * @brief Draws a word using XPM letters on the screen.
 * 
 * @param sprite XPM image containing the alphabet.
 * @param img_addr Address of the image.
 * @param x X-coordinate for the top-left corner of the first letter.
 * @param y Y-coordinate for the top-left corner of the first letter.
 * @param word Word to be drawn.
 * 
 * @return Returns 0 upon success and 1 upon failure.
 */
int (draw_xpm_word)(xpm_image_t sprite, uint8_t* img_addr, uint16_t x, uint16_t y, char* word);

/**
 * @brief Draws a number using an XPM image on the screen.
 * 
 * @param sprite XPM image containing the numbers.
 * @param img_addr Address of the image.
 * @param x X-coordinate for the top-left corner of the number.
 * @param y Y-coordinate for the top-left corner of the number.
 * @param number Number to be drawn.
 * 
 * @return Returns 0 upon success.
 */
int (draw_number)(xpm_image_t sprite, uint8_t* img_addr, uint16_t x, uint16_t y, char number);

#endif
