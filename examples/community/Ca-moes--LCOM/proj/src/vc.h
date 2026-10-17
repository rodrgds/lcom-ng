#pragma once
#include <lcom/lcf.h>
#include <sentences.h>

/**
 * @brief Return VBE Mode Information
 * 
 * @param mode mode whose information should be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * 
 * @return 0 on success, non-zero otherwise
*/
int vbe_get_info_mode(uint16_t mode, vbe_mode_info_t * vmi_p);

/**
 * @brief Sets the Graphics Mode
 * 
 * @param mode the graphics mode to set
 * 
 * @return 0 on success, non-zero otherwise
 */
int graphics_mode(uint16_t mode);

/**
 * @brief Get horizontal resolution 
 * 
 * @return horizontal resolution
*/
unsigned get_hres();

/**
 * @brief Get vertical resolution 
 * 
 * @return vertical resolution
*/
unsigned get_vres();

/**
 * @brief Draws an XPM on the screen at specified coordinates.
 * 
 * @param sprite sprite
 * @param x horizontal coordinate of upper-left corner
 * @param y vertical coordinate of upper-left corner
 * @param img xpm image to draw
 * @param buff linear buffer  
 * 
 * @return 0 on success, non-zero otherwise
 */
int draw_xpm_image(uint8_t *sprite, uint16_t x, uint16_t y, xpm_image_t img, void * buff);

/**
 * @brief Draws sentence on the screen at specified coordinates 
 * 
 * Also sets the color of each letter taking into account the ones that were correctly typed by the * user and the ones that were not. Avoids breaking words by measuring width between two spaces.
 * 
 * @param sentence the sentence
 * @param x horizontal coordinate of upper-left corner
 * @param y vertical coordinate of upper-left corner
 * @param buff linear buffer  
 * @param errors the number of errors
 * @param correct the index of the last letter typed correctly
 * 
 * @return 0 on success, non-zero otherwise
 */
int draw_sentence(letter *sentence, uint16_t x, uint16_t y, void * buff,uint8_t errors, int correct);

/**
 * @brief Draws the name of a player (username)
 * 
 * @param sentence the name of the user
 * @param x horizontal coordinate of upper-left corner
 * @param y vertical coordinate of upper-left corner
 * @param buff linear buffer  
 * @param n number of letter of the username
 * 
 * @return 0 on success, non-zero otherwise
 */
int draw_name(letter *sentence, uint16_t x, uint16_t y, void * buff, unsigned int n);

/**
 * @brief Draws the input from the user on the screen at specified coordinates
 * 
 * Also sets the color of each letter taking into account the ones that were correctly typed by the * user and the ones that were not. Avoids breaking words by measuring width between two spaces.
 * 
 * @param input the input from the user
 * @param sentence the sentence
 * @param x horizontal coordinate of upper-left corner
 * @param y vertical coordinate of upper-left corner
 * @param buff linear buffer  
 * @param errors the number of errors
 * @param correct the index of the last letter typed correctly
 * 
 * @return 0 on success, non-zero otherwise
 */
int draw_input_sentence(letter *input,letter *sentence, uint16_t x, uint16_t y, void * buff,uint8_t errors, int correct);

/**
 * @brief Draws a letter on the screen
 * 
 * @param sprite letter's sprite
 * @param x horizontal coordinate of upper-left corner
 * @param y vertical coordinate of upper-left corner
 * @param img xpm image to draw
 * @param buff linear buffer  
 * @param color the color of the letter
 * 
 * @return 0 on success, non-zero otherwise
 */
int draw_letter(uint8_t *sprite, uint16_t x, uint16_t y, xpm_image_t img, void * buff, void * color);

/**
 * @brief Draw a pixel on the screen
 * 
 * @param x horizontal coordinate of upper-left corner
 * @param y vertical coordinate of upper-left corner
 * @param color the color of the letter
 * @param buff linear buffer
*/
void draw_pixel (uint16_t x, uint16_t y, const void  *color, void * buff);

/** 
 * @brief Get double buffer
 * 
 * @return Double buffer address
 */
char* get_double_buffer();

/** 
 * @brief Perform double buffering - copy contents of the back buffer to the frame buffer
 */
void double_buffering();

/** 
 * @brief Free memory allocate for the double buffer
 */
void free_double_buffer();
