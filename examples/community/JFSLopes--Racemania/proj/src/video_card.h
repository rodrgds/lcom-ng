#ifndef KDB_LAB_3
#define KDB_LAB_3

#include <lcom/lcf.h>
#include "sprites.h"

/**
 * @brief Changes the mode the VBE is working
 * 
 * @param mode The mode to be set
 * 
 * @return 0 on success, -1 otherwise
 * 
*/
int (change_mode)(uint16_t mode);

/**
 * @brief Changes the VBE mode and maps the VRAM to the proccess address space
 * 
 * @param mode The mode to be set
 * 
 * @return 0 on success, -1 otherwise
*/
int (vg_start) (uint16_t mode);

/**
 * @brief Ajust the the color depending on the mode
 * 
 * Initially, each color component has 8 bits, but some modes use fewer bits, so this function adjusts the values.
 * 
 * @param color Color to be ajusted
*/
void (ajust_color) (uint32_t* color);

/**
 * @brief Draws the pixel in the coordinates (pos_x, pos_y)
 * 
 * @param pos_x The x-axis coordinate
 * @param pos_y The y-axis coordinate
 * @param color Pixel to be drawn
 * 
 * @return 0 on success, -1 otherwise
*/
int (vg_draw_pixel) (uint16_t pos_x, uint16_t pos_y, uint32_t color);

/**
 * @brief Draw a line on (x, y) with lenght 'len'
 * 
 * @param x The x-axis coordinate
 * @param y The y-axis coordinate
 * @param len The lenght of the line
 * @param color The color of the line
 * 
 * @return 0 on success, -1 otherwise
*/
int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle on (x, y) with dimension width and height
 * 
 * @param x The x-axis coordinate
 * @param y The y-axis coordinate
 * @param width The rectangle width
 * @param height The rectangle height
 * @param color The rectangle color
 * 
 * @return 0 on success, -1 otherwise
*/
int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a matrix pattern with dimensions (no_rectangles, no_rectangles)
 * 
 * @param no_rectangles Dimension of the matrix
 * @param first The color of the first matrix entry
 * @param step Is a value used to calculate the next entry color
 * 
 * @return 0 on success, -1 otherwise
*/
int (draw_matrix) (uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Draws a xpm
 * 
 * @param xpm sprite that represents the image that will be draw
 * 
 * @return 0 on success, -1 otherwise
*/
int (show_xpm) (sprite* xpm);

/**
 * @brief Erases a xpm 
 * 
 * @param xpm sprite that represents the image that will be draw
 * 
 * @return 0 on success, -1 otherwise
*/
int (clear_xpm) (sprite* xpm);

/**
 * @brief Rotates a xpm
 * 
 * @param xpm sprite that represents the image that will be draw
 * 
*/
void (rotate) (sprite* xpm);

/**
 * @brief Displays an XPM sprite at the specified position.
 * 
 * @param xpm Pointer to the sprite structure.
 * @param posx X-coordinate where the sprite will be displayed.
 * @param posy Y-coordinate where the sprite will be displayed.
 * @return int Returns 0 on success, non-zero otherwise.
 */
int (show_xpm_at_position) (sprite* xpm, uint16_t posx, uint16_t posy);

/**
 * @brief Displays an XPM image at the specified position.
 * 
 * @param info Pointer to the xpm_image_t structure containing the image information.
 * @param pos_x X-coordinate where the image will be displayed.
 * @param pos_y Y-coordinate where the image will be displayed.
 * @return int Returns 0 on success, non-zero otherwise.
 */
int (show_xpm1) (xpm_image_t *info, uint16_t pos_x, uint16_t pos_y);

/**
 * @brief Rotates an XPM sprite at the specified position.
 * 
 * @param xpm Pointer to the sprite structure.
 * @param posx X-coordinate where the sprite will be rotated.
 * @param posy Y-coordinate where the sprite will be rotated.
 */
void (rotate_at_position) (sprite* xpm, uint16_t posx, uint16_t posy);

/**
 * @brief Moves the mouse cursor according to the provided packet data.
 * 
 * @param cursor Pointer to the mouse_sprite structure representing the cursor.
 * @param pp Pointer to the packet structure containing the mouse movement data.
 * @return int Returns 0 on success, non-zero otherwise.
 */
int (move_mouse_cursor) (mouse_sprite* cursor, struct packet* pp);

/**
 * @brief Clears the screen.
 */
void (clean_screen) ();

/**
 * @brief Displays the mouse cursor on the screen.
 * 
 * @param cursor Pointer to the mouse_sprite structure representing the cursor.
 * @return int Returns 0 on success, non-zero otherwise.
 */
int (show_mouse_cursor) (mouse_sprite* cursor);

/**
 * @brief Clears the mouse cursor from the screen.
 * 
 * @param cursor Pointer to the mouse_sprite structure representing the cursor.
 * @return int Returns 0 on success, non-zero otherwise.
 */
int (clear_mouse_cursor) (mouse_sprite* cursor);

/**
 * @brief Displays a race track sprite at the specified position.
 * 
 * @param race Pointer to the sprite structure representing the race track.
 * @param car_x_pos X-coordinate where the race sprite will be displayed.
 * @param car_y_pos Y-coordinate where the race sprite will be displayed.
 * @return int Returns 0 on success, non-zero otherwise.
 */
int (show_race) (sprite* race, uint16_t car_x_pos, uint16_t car_y_pos);

/**
 * @brief Copies the current buffer to the video memory.
 */
void (copy_to_video_mem) ();

/**
 * @brief Checks for a collision with the car sprite.
 * 
 * @param car Pointer to the sprite structure representing the car.
 * @return int Returns CRASH if a collision is detected, NO_COLLISION otherwise.
 */
int (car_collision(sprite* car));

/**
 * @brief Loads a background sprite.
 * 
 * @param background Pointer to the sprite structure representing the background.
 */
void (load_background)(sprite* background);

#endif
