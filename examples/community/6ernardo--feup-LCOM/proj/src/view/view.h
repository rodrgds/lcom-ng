#ifndef _LCOM_VIEW_H_
#define _LCOM_VIEW_H_
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/video/graphics.h"
#include "../controller/rtc/rtc.h"
#include "model/model.h"
#include "model/xpm/numberZero.xpm"
#include "model/xpm/numberOne.xpm"
#include "model/xpm/numberTwo.xpm"
#include "model/xpm/numberThree.xpm"
#include "model/xpm/numberFour.xpm"
#include "model/xpm/numberFive.xpm"
#include "model/xpm/numberSix.xpm"
#include "model/xpm/numberSeven.xpm"
#include "model/xpm/numberEight.xpm"
#include "model/xpm/colon.xpm"

/** @defgroup view view
 * @{
 *
 * Functions for drawing the game
 */

/**
 * @brief Draws the mouse cursor based on the current state and position.
 *
 * The draw_mouse() function is responsible for rendering the mouse cursor on the screen. The appearance of the cursor depends on various conditions, including whether the mouse is over a clickable area or the game state and tool type.
 * If the mouse is over a clickable area, a hand cursor sprite is drawn. In the game state, different cursors are displayed based on the selected tool: pencil, eraser, bucket, or a default mouse cursor.
 
 */
 

void draw_mouse();

/**
 * @brief Sets up the frame buffers for the specified mode.
 * @param mode Mode to be used.
 *
 * The set_frame_buffers() function initializes the frame buffers based on the specified mode. It first calls the set_frame_buffer() function to set up the main frame buffer. 
 * If the DOUBLE_BUFFER flag is enabled, the function allocates memory for the secondary_frame_buffer, third_frame_buffer,
 * and backup_frame_buffer. These buffers are used for double buffering, where one buffer is used for rendering while
 * the other is displayed. This technique helps reduce flickering and provides smoother animations. 
 */


int set_frame_buffers(uint16_t mode);

/**
 * @brief Swaps the contents of the main frame buffer and the secondary frame buffer.
 */

void swap_buffers();

/**
 * @brief Copies the contents of the secondary frame buffer to the third frame buffer.
 */


void copyTheDraw();

/**
 * @brief Clears the contents of the secondary frame buffer.
 */

void cleanSecondaryBuffer();

/**
 * @brief Clears the contents of the third frame buffer.
 */

void cleanThirdBuffer();

/**
 * @brief Clears the contents of the third frame buffer.
*/
void cleanThirdBuffer2(uint32_t drawingColor);

/**
 * @brief Clears the contents of the backup frame buffer.
 */
void cleanBackupBuffer();

/**
 * @brief Draws a sprite on the screen at the specified coordinates.
 * @param sprite Sprite to be drawn.
 * @param x X coordinate of where to be draw.
 * @param y Y coordinate of where to be draw.
 * 
 *
 * The draw_sprite_xpm() function takes a Sprite structure pointer representing the sprite to be drawn, and the (x, y) coordinates
 * where the sprite should be positioned on the screen. The function retrieves the color information from the sprite's colors array
 * and uses the paint_pixel() function to draw each pixel of the sprite at the corresponding position on the secondary frame buffer.
 */

int draw_sprite_xpm(Sprite *sprite , int x , int y);

/**
 * @brief Draws the game menu on the screen.
 *
 * The draw_game_menu() function is used to draw the game menu on the screen. It calls the draw_sprite_xpm() function to draw
 * individual sprites representing different elements of the game menu at specific coordinates on the screen.
 */

void draw_game_menu();
/**
 * @brief Draws a single frame on the screen.
 *
 * The draw_frame() function is responsible for drawing a single frame on the screen based on the current game state. It performs
 * different drawing operations depending on the game state.
 */
void draw_frame();

/**
 * @brief Draws the game elements on the screen.
 *
 * This function draws various sprites representing numbers and letters for the game.
 * The sprites are positioned at specific coordinates on the screen.
 *
 * @return 0 indicating successful drawing of the game elements.
 */


int draw_game();

/**
 * @brief Draws the header section of the game screen.
 *
 * The draw_header() function is responsible for drawing the header section of the game screen. It initializes an array of colors
 * representing the available drawing colors. It fills the top portion of the secondary frame buffer with a light gray color
 * (0xDDDDDD) to serve as the header background.
 *
 * The function then draws rectangular color swatches and icons for the eraser and bucket tools. It iterates through the color
 * array and calls the drawColor() function to draw rectangular color swatches at specific positions on the header. The x coordinate
 * is incremented by 79 pixels for each color swatch to create a spaced layout. The eraser and bucket icons are drawn after the
 * color swatches.
 */

void draw_header();

/**
 * @brief Draws a solid color rectangle on the secondary frame buffer.
 * @param x X coordinate of the rectangle.
 * @param y Y coordinate of the rectangle.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color of the rectangle.
 */



void drawColor(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int sprite_clicked(Sprite *sprite, int x, int y);



/**
 * @brief Draws a circular drawing pointer with the specified radius and color.
 * @param x The x-coordinate of the center of the circular pointer.
 * @param y The y-coordinate of the center of the circular pointer.
 * @param radius The radius of the circular pointer.
 * @param color The color of the circular pointer.
 * @return 0 on success, indicating that the circular pointer was drawn successfully.
 */


int draw_line_pointer(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);

/**
 * @brief Draws a line segment between two points in each mouse interrupt on the screen.
 *
 * @param xi The x-coordinate of the start point.
 * @param yi The y-coordinate of the start point.
 * @param xf The x-coordinate of the end point.
 * @param yf The y-coordinate of the end point.
 * @param color The color of the line segment.
 *
 * @return 0 if the line segment was drawn successfully, 1 if there was an error while drawing.
 */


int drawLineSegment(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color);

/**
 * @brief Process drawing operation between two points.
 * @param x The x-coordinate of the current point.
 * @param y The y-coordinate of the current point.
 * @param prevX The x-coordinate of the previous point.
 * @param prevY The y-coordinate of the previous point.
 * @param drawingColor The color to be used for drawing.
 *
 * @return 0 on success, indicating that the drawing operation was successful.
 * @return 1 if the current point is outside the valid drawing region.
 */
int processDrawing(int x, int y,int prevX, int prevY, uint32_t drawingColor);

/**
 * @brief Updates the clock display on the screen.
 *
 * The updateClock() function retrieves the current time information and uses a set of number sprites to represent the hours, minutes, and seconds on the screen. It calculates the individual digits of the time values and draws the corresponding sprites at the appropriate positions on the screen.
 */


void updateClock();

/**
 * @brief Initializes the number sprites used for displaying digits.
 *
 * The initialize_number_sprites() function assigns the number sprite images to the corresponding elements in the number_sprite array. Each sprite represents a digit from 0 to 9.
 */

void initialize_number_sprites();



/**
 * @brief Processes the drawing of a rectangle.
 *
 * The processRectangle() function handles the drawing of a rectangle based on the given coordinates and drawing color. It checks if the y-coordinate is within a valid range and then determines the position and size of the rectangle based on the previous and current coordinates. The draw_rectangle() function is called to draw the rectangle on the third frame buffer.
 *
 * @param x The x-coordinate of the current position.
 * @param y The y-coordinate of the current position.
 * @param prevX The x-coordinate of the previous position.
 * @param prevY The y-coordinate of the previous position.
 * @param drawingColor The color of the rectangle to be drawn.
 *
 * @return 0 if the rectangle was drawn successfully, 1 otherwise.
 */

int processRectangle(int x, int y,int prevX, int prevY, uint32_t drawingColor);

/**
 * @brief Inverts the color of the pixels on the third frame buffer.
 */

void invertColor();

/**
 * @brief Converts the colors of the current drawing to grayscale.
 */

void gray_components();

/**
 * @brief Backs up the current drawing to a backup frame buffer.
 *
 * The backup_draw() function copies the contents of the current drawing in the third frame buffer to a backup frame buffer. It uses the memcpy() function to perform a memory copy operation.
 */

void backup_draw();

/**
 * @brief Inverts the backup frame buffer to the current drawing.
 *
 * The invert_backup_draw() function copies the contents of the current drawing in the third frame buffer to the backup frame buffer. It uses the memcpy() function to perform a memory copy operation.
 */

void invert_backup_draw();

/**
 * @brief Draws the instructions page based on the current inst_page value.
 * 
 * If inst_page is 1, it draws the first instruction page along with the next arrow button.
 * If inst_page is 2, it draws the second instruction page along with the previous arrow button.
 */

void draw_instructions();


/**
 * @brief Draws the outline of the selected drawing tool.
 *
 * The draw_tool_outline() function checks the current selected drawing tool and draws the outline of the tool accordingly. It uses the draw_sprite_xpm() function to draw the outline sprite at the specified position.
 */

void draw_tool_outline();

#endif
