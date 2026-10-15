#ifndef VIEW_H
#define VIEW_H

#include "../macros/includes.h"
#include "../devices/graphics.h"
#include "../devices/timer_proj.h"
#include "../game_state/level.h"

/**
 * @brief Sets all entities and items to dead so that they will not show on screen anymore.
 * 
 */
void eliminate_entities();

/**
 * @brief Calls all the initializing functions (the ones with the prefix "init"). The initializing functions load the xpms and stores them in a struct img_t. The creation and storage of an AnimatedImg_t struct is also used for entities with animation.
 * 
 */
void initialize_game();

/**
 * @brief Loads and stores mouse xpm and other mouse related variables.
 * 
 */
void init_mouse();
/**
 * @brief Sets all level variables and entities to their starting values.
 * 
 */
void init_level();
/**
 * @brief Loads and stores monster xpm and other monster related variables.
 * 
 */
void init_monsters();
/**
 * @brief Loads and stores packages xpm and other package related variables.
 * 
 */
void init_packages();
/**
 * @brief Loads and stores teleporter xpm and other teleporter related variables.
 * 
 */
void init_teleports();
/**
 * @brief Loads and stores inventory xpm and other inventory related variables.
 * 
 */
void init_inventory();
/**
 * @brief Loads and stores item xpm and other item related variables.
 * 
 */
void init_items();
/**
 * @brief Loads and stores bullet xpm and other bullet related variables.
 * 
 */
void init_bullets();
/**
 * @brief loads and stores animated xpms of the player on the struct AnimatedImg_t
 * 
 */
void init_anim_img();
/**
 * @brief Loads and stores number xpm and other number related variables.
 * 
 */
void init_numbers();
/**
 * @brief Loads and stores letters and symbols xpm and other related variables.
 * 
 */
void init_chars();
/**
 * @brief Loads and stores single item xpm and other related variables.
 * 
 */
void init_misc();
/**
 * @brief Loads and stores player xpm and other player related variables.
 * 
 */
void init_player();

/**
 * @brief Helper function to set position and draw letters and symbols.
 * @param img entity that will be moved and drawn
 * @param x horizontal coordinate of final position
 * @param y vertical coordinate of final position
 * @param type type of coloring. If it's 0 then the coloring is white, if 1 the coloring is green, if 4 coloring is red.
 * 
 */
void set_char(img_t *img, int x, int y, int type);



/**
 * @brief Loads and sets the variables of struct img_t of a xpm.
 * @param xpm The xpm to be loaded and stored
 * @param x horizontal coordinate of img_t position
 * @param y vertical coordinate of img_t position
 * 
 */

img_t make_img(xpm_map_t xpm,unsigned int x, unsigned int y);

/**
 * @brief Loads all animated entities xpms into a struct called AnimatedImg_t.
 * @param no_images The number of different xpms used for every direction the image faces
 * @param speed The velocity on which the entity moves
 * @param up Array which stores the xpms of the entity while facing up
 * @param left Array which stores the xpms of the entity while facing left
 * @param right Array which stores the xpms of the entity while facing right
 * @param down Array which stores the xpms of the entity while facing down
 */
AnimatedImg_t make_animated_img(int no_images, int speed, xpm_map_t *up, xpm_map_t *left, xpm_map_t *right, xpm_map_t *down);

/**
 * @brief Loads all simple animated entities xpm into a struct simple_animation_t.
 * @param no_images The number of different xpms used for the entity
 * @param animation The array with the different xpms of the animation
 */
simple_animation_t make_simple_animation(int no_images, xpm_map_t *animation);

/**
 * @brief Checks in the prev_buffer for collisions
 * @param img The entity that is currently moving
 * @param distance The distance that it will move if there is no collision
 * @return Returns 0 if there is no collision and 1 if otherwise
 */
int is_occupied(img_t img, int distance);
/**
 * @brief Checks in the frame_buffer for collisions
 * @param img The entity that is currently moving
 * @param distance The distance that it will move if there is no collision
 * @return Returns 0 if function was successfull
 */
int manage_collision(img_t *img, int distance);
/**
 * @brief Detects if character passed through by argument is in contact with another entity also passed by argument
 * @param target Entity which is being collided by character
 * @param character Entity that is colliding
 * @return Returns true if it is colliding and false if otherwise
 */
bool manage_hitbox(img_t target, img_t character);

/**
 * @brief Eliminates monster in a certain position passed by argument
 * @param x horizontal coordinate of monster position
 * @param y vertical coordinate of monster position
 */
void hit_monster(int x, int y);
/**
 * @brief Eliminates wolf in a certain position passed by argument
 * @param x horizontal coordinate of wolf position
 * @param y vertical coordinate of wolf position
 * @return Returns true if wolf is in the position and false if otherwise.
 */
bool hit_wolf(int x, int y);

/**
 * @brief Calls drawing function on frame_buffer on each pixel of image passed by argument 
 * @param draw_img Colors, width, height of image
 * @param img coordinates of the position of image
 * @return Returns 0 if successfull and 1 otherwise
 */
int draw(img_t draw_img, img_t img);
/**
 * @brief Calls drawing function on map buffer on each pixel of image passed by argument 
 * @param draw_img Colors, width, height of image
 * @param img coordinates of the position of image
 * @return Returns 0 if successfull and 1 otherwise
 */
int draw_map(img_t draw_img, img_t img);
/**
 * @brief Calls drawing function on frame_buffer on each pixel of image passed by argument on char images (letters, symbols, numbers)
 * @param number Colors, width, height, coordinates of the position of image
 * @param type The type of coloring of image. If 0 then it's normal coloring, if 1 then it's green coloring and if 4 it is red coloring
 * @return Returns 0 if successfull and 1 otherwise
 */
int draw_numbers(img_t number, int type);
/**
 * @brief Loads and calls drawing function on frame_buffer on each pixel of xpm passed by argument
 * @param xpm Colors, width, height of xpm
 * @param x horizontal coordinate of xpm position
 * @param y vertical coordinate of xpm position
 * @return Returns 0 if successfull and 1 otherwise
 */
int draw_small_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);
/**
 * @brief Loads and calls drawing function on map buffer of the background xpm
 * @return Returns 0 if successfull and 1 otherwise
 */
int drawTiles();
/**
 * @brief Loads and calls drawing function on map buffer of the border xpm
 * @return Returns 0 if successfull and 1 otherwise
 */
int drawBorder();

/**
 * @brief Calls drawing function to draw the ongoing timer based on the values passed by argument
 * @param sec The value of the seconds of the timer
 * @param min The value of the minutes of the timer
 * @return Returns 0 if successfull and 1 otherwise
 */
int draw_timer(int sec, int min);
/**
 * @brief Calls drawing function to draw a number passed by argument
 * @param number Value of number to be drawn
 * @param x horizontal coordinate of number position
 * @param y vertical coordinate of number position
 * @param type Type of coloring format. If 0 then it is normal coloring, if 1 then it is green coloring, if 4 then it is red coloring.
 * @return Returns 0 if successfull and 1 otherwise
 */
int draw_number(int number, int x, int y, int type);
/**
 * @brief Calls drawing function to draw the health of the player
 * @return Returns 0 if successfull
 */
int draw_health();
/**
 * @brief Calls number drawing function with image and coordinates passed by argument and number value based on the specific image number associated with it
 * @param img The image to be drawn
 * @param x horizontal coordinate of image position
 * @param y vertical coordinate of image position
 * @return Returns 0 if successfull
 */
int draw_img_number(img_t img, int x, int y);
/**
 * @brief Calls drawing function on frame_buffer to draw a line of format passed by argument
 * @param x horizontal coordinate of line position
 * @param y vertical coordinate of line position
 * @param len The length of the line to be drawn
 * @param colors The color that will fill the line
 * @return Returns 0 if successfull and 1 otherwise
 */
int draw_line(int x, int y, int len, uint32_t colors);

#endif
