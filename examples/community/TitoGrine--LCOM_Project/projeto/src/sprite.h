#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "graphics.h"
#include "queue.h"


/** A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits.
 */
typedef struct {
  int x;               /**< Current sprite position on the X axis. */
  int y;               /**< Current sprite position on the Y axis. */
  int width;           /**< Sprite width. */
  int height;          /**< Sprite height. */
  int xspeed;          /**< Speed on the X axis. */
  int yspeed;          /**< Speed on the Y axis. */
  char *map;           /**< Sprite pixmap (use read_xpm()). */
} Sprite;

/** Creates a new sprite with pixmap "pic", with specified
 * position (within the screen limits) and speed;
 * Does not draw the sprite on the screen
 * Returns NULL on invalid pixmap.
 */
Sprite * create_sprite(const char *pic[], int x, int y,int xspeed, int yspeed);

/** The "fig" sprite is erased from memory whose address is "base"
 * and used resources released.
 */
void destroy_sprite(Sprite *fig);

/**
 * @brief Adds a sprite's x and y speeds to it's x and y coordinates,
 * respectively.
 * 
 * @param fig     Sprite to be moved
 */
void move_xpm(Sprite *fig);

/**
 * @brief Changes a sprites position to the given coordinates.
 * 
 * @param sp          Sprite to be moved
 * @param x_pos       New horizontal position
 * @param y_pos       New vertical position
 */
void change_position(Sprite* sp, uint16_t x_pos, uint16_t y_pos);

/**
 * @brief Draws the given sprite using the function paint_pixel(), 
 * doesn't paint pixels with the transparency color.
 * 
 * @param sp        Sprite to be painted
 */
void draw_Sprite(Sprite* sp);

/**
 * @brief Draws the given sprite using the function paint_pixel(),
 * however it ignores the sprites color values and only paints the 
 * given color or nothing if the color value in the sprite is the
 * transparency_color. Used for painting in the canvas.
 * 
 * @param sp        Sprite to be painted
 * @param color     Color that will be painted
 */
void paint_Sprite(Sprite* sp, int color);

/**
 * @brief Erases a sprite replacing the values for the background xpm
 * ones. Even though it only replaces the values in the current sprite
 * position, this function should be used as little as possible since
 * the xpm_load function it uses takes up memory and a long time to
 * execute, slowing th program.
 * 
 * @param sp        Sprite to be erased
 */
void erase_Sprite(Sprite* sp);

/**
 * @brief Works the same way as erase_Sprite except instead of 
 * replacing the values for the background, it replaces them with
 * the xpm passed as argument. This funtion should still be avoided
 * for the same reasons as erase_Sprite.
 * 
 * @param sp        Sprite to be erased
 * @param wallpaper Current background to replace sprite values
 */
void erase_dynamic(Sprite* sp, xpm_string_t *wallpaper);

/**
 * @brief Erases the cursors previous location, adds to the cursor
 * position the vales move_x and move_y. Paints the cursor again with
 * the given color.
 * 
 * @param sp        Cursor's sprite
 * @param move_x    Horizontal movement of the cursor
 * @param move_y    Vertical movement of the cursor
 * @param color     Color of the cursor
 */
void move_cursor(Sprite* sp, int16_t move_x, int16_t move_y, int color);

/**
 * @brief Draws the cursor sprite. If the sprite's value in a given 
 * position is the transparency color it doesn't paint it. If it's
 * the color 0xFFFFFE, it paints the color passed as argument instead
 * of the sprites actual color. Used for painting cursor sprites,
 * enabling the user to see th current paint color. Uses the 
 * paint_pixel_cursor function that doesn't write to the previous array.
 * 
 * @param sp        Cursor's sprite to be drawn
 * @param color     Possible cursor color depending on the type
 */
void draw_cursor(Sprite *sp, int color);

/**
 * @brief Erases the cursor's sprite replacing for the color values
 * in the previous array, that holds every color change except the
 * cursor ones.
 * 
 * @param sp        Cursor's sprite to be erased
 */
void erase_cursor(Sprite *sp);

/**
 * @brief Given a sprite, a horizontal displacement and a vertical
 * displacement it calculates each axis displacement per iteration
 * and fills the given displacement with the given sprite with the
 * given color. Uses draw_Sprite function.
 * 
 * @param sp         Cursor's sprite to be drawn
 * @param delta_x    Horizontal displacement 
 * @param delta_y    Vertical displacement
 * @param color      Color of sprite to be drawn
 */
void fill_displacement(Sprite* sp, int16_t delta_x, int16_t delta_y, int color);

 /**
  * @brief Works the same as the fill_displacement function but instead
  * of using the draw_Sprite, it uses random_fill function and thus does
  * not need a sprite as an argument.
  * 
  * @param x_pos      Initial horizontal coordenate
  * @param y_pos      Initial vertical coordenate
  * @param delta_x    Horizontal displacement
  * @param delta_y    Vertical displacement
  * @param color      Color used for painting
  */
void spray_area(uint16_t x_pos, uint16_t y_pos, int16_t delta_x, int16_t delta_y, int color);

/**
 * @brief In a default circle xpm it checks every value, if it's the
 * trasnparency color it doesn't paint, if not it has a 1/50 chance of
 * painting that pixel. It's used to mimic a spray effect when painting.
 * 
 * @param x_pos       Horizontal coordinate
 * @param y_pos       Vertical coordinate
 * @param color       Color to be painted
 */
void random_fill(uint16_t x_pos, uint16_t y_pos, int color);

/**
 * @brief Using a queue it checks every pixel in around the initial
 * pixel until it finds a border of pixels with a color different from
 * the initial background's pixel color. It paints every pixel inside
 * that border with the given color. Used for bucket painting 
 * functionality.
 * 
 * @param x_pos        Initial horizontal coordenate
 * @param y_pos        Initial vertical coordenate
 * @param color        Color to be painted
 */
void fill_area(uint16_t x_pos, uint16_t y_pos, int color);

/**
 * @brief Checks if the pixel in the screen on the given x and y 
 * position has the given color. Used for fill functionality.
 * 
 * @param x_pos        Pixel's horizontal position
 * @param y_pos        Pixel's vertical position
 * @param fl_color     Color to check
 * 
 * @return true        Pixel is the given color
 * @return false       Pixel has a different color from the given
 */
bool check_pixel(uint16_t x_pos, uint16_t y_pos, int fl_color);

/**
 * @brief Checks if a sprite is within the limits of the screen.
 * 
 * @param sp           Sprite to be checked
 * @param x_pos        Horizontal position
 * @param y_pos        Vertical position
 * 
 * @return true        Sprite is within the limits of the screen 
 * @return false       Sprite is outside the limits of the screen
 */
bool check_limits(Sprite* sp, uint16_t x_pos, uint16_t y_pos);

/**
 * @brief Given a button sprite and x and y coordinates checks if
 * those coordinates are within the limits of the button.
 * 
 * @param button       Button's sprite
 * @param x_pos        Horizontal position
 * @param y_pos        Vertical position
 * 
 * @return true        Pixel in given coordinates is within the button 
 * @return false       Pixel in given coordinates isn't within the button
 */
bool button_hovering(Sprite* button, uint16_t x_pos, uint16_t y_pos);

/**
 * @brief Given a pallete sprite, checks if the pixel in the given
 * x and y coordinate are within the gradient of the pallete, it
 * accounts for the border of the pallete.
 * 
 * @param pallete      Pallete sprite
 * @param x_pos        Horizontal position
 * @param y_pos        Vertical position
 *  
 * @return true        Pixel in given coordinates is within the pallete 
 * @return false       Pixel in given coordinates isn't within the pallete
 */
bool pallete_hovering(Sprite *pallete, uint16_t x_pos, uint16_t y_pos);

/**
 * @brief Given a canvas sprite, checks if the brush sprite in the given
 * x and y coordinate are within the paintable part of the canvas ,it
 * accounts for the border of the canvas.
 * 
 * @param canvas       Canvas sprite
 * @param brush        Brush sprite
 * @param x_pos        Horizontal position
 * @param y_pos        Vertical position
 * 
 * @return true        Pixel in given coordinates is within the canvas 
 * @return false       Pixel in given coordinates isn't within the canvas 
 */
bool in_canvas(Sprite* canvas, Sprite* brush,  uint16_t x_pos, uint16_t y_pos);

/**
 * @brief Given a sprite with the given amount of x momentum, checks if
 * it will leave the borders of the screen, if not it returns the given
 * speed, otherwise it calculates the maximum displacement possible and
 * returns it.
 * 
 * @param sp          Object's sprite
 * @param x_speed     Horizontal momentum
 * 
 * @return int16_t    Given displacement or maximum possible
 */
int16_t calc_speed_x(Sprite *sp, int16_t x_speed);

/**
 * @brief Given a sprite with the given amount of y momentum, checks if
 * it will leave the borders of the screen, if not it returns the given
 * speed, otherwise it calculates the maximum displacement possible and
 * returns it.
 * 
 * @param sp          Object's sprite
 * @param x_speed     Vertical momentum
 * 
 * @return int16_t    Given displacement or maximum possible
 */
int16_t calc_speed_y(Sprite *sp, int16_t y_speed);

#endif
