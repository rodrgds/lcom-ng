#pragma once

#include <lcom/lcf.h>
#include "rtc.h"

struct Point;

/**
 * @brief Struck that contains all the information of a game sprite.
 */
typedef struct {
    /** @brief  x is the current positions on horizontal axe of the top left corner of the sprite in the screen (correspond to a pixel x coordenate)*/
    int x;
    /** @brief  y is the current positions on vertical axe of the top left corner of the sprite in the screen (correspond to a pixel y coordenate)*/
    int y;  
    /** @brief  width is the horizontal dimension of the sprite in pixels*/
    int width;
    /** @brief  height is the vertical dimension of the sprite in pixels*/
    int height; 
    /** @brief  xspeed is the current speed of the sprite in the horizontal axe*/
    int xspeed;
    /** @brief  yspeed is the current speed of the sprite in the vertical axe*/
    int yspeed; 
    /** @brief  map corresponds to the pixmap currently used*/
    uint32_t *map; 
    /** @brief  map_animates is an array of pixmaps for each position of the sprite*/
    uint32_t **map_animates;
    /** @brief  n_animations number of pixmaps on the map_animates, map_animates size*/
    int n_animations;
    /** @brief  actual_animation_indx is the index on the map_animates array to the map that is currently in use*/
    int actual_animation_idx;
} Sprite;

/**
 * @brief Struck that contains the map to the a pixmap (map) and its dimensions used in menus or middle game screens
 */
typedef struct
{
    /** @brief  width and height are the dimensions of the sprite in pixels*/
    uint32_t *map; 
    /** @brief  width is the horizontal dimension of the sprite in pixels*/
    int width;
    /** @brief  height is the vertical dimension of the sprite in pixels*/
    int height;
} MenuSprite;

/**
 * @brief Struck that contains all the information of the cursor sprite.
 */
typedef struct 
{
    /** @brief  x is the current positions on horizontal axe of the top left corner of the sprite in the screen (correspond to a pixel x coordenate)*/
    int x;
    /** @brief  y is the current positions on vertical axe of the top left corner of the sprite in the screen (correspond to a pixel y coordenate)*/
    int y;
    /** @brief  width is the horizontal dimension of the sprite in pixels*/
    int width;
    /** @brief  height is the vertical dimension of the sprite in pixels*/
    int height;
    /** @brief map correspond to the pixmap currently used*/
    uint32_t *map; 
    /** @brief width and height are the dimensions of the sprite in pixels*/
    uint32_t **map_animates; 
    /** @brief map_animates is an array of pixmaps for each position of the sprite*/
    int n_animations;
    /** @brief actual_animation_indx is the index on the map_animates array to the map that is currently in use*/
    int actual_animation_idx;
} CursorSprite;

/**
 * @brief Create a new sprite object 
 * 
 * @param xpm pixmap
 * @param x position in horizontal axe
 * @param y position in vertical axe
 * @param xspeed speed in horizontal axe
 * @param yspeed speed in vertical axe
 * @param enumtype with mode used
 * @return Sprite* to the new sprite and null on invalid pixmap
 */
Sprite *create_sprite(xpm_map_t xpm, int x, int y,int xspeed, int yspeed, enum xpm_image_type enumtype);

/**
 * @brief Create a new menu sprite object
 * 
 * @param xpm pixmap
 * @param enumtype with mode used
 * @return MenuSprite* to the new sprite and null on invalid pixmap
 */
MenuSprite * create_menu_sprite(xpm_map_t xpm,enum xpm_image_type enumtype);

/**
 * @brief Create a new cursor sprite object
 * 
 * @param xpm pixmap
 * @param x position in horizontal axe
 * @param y position in vertical axe
 * @param enumtype with mode used
 * @return CursorSprite* to the new sprite and null on invalid pixmap
 */
CursorSprite * create_cursor_sprite(xpm_map_t xpm, int x, int y, enum xpm_image_type enumtype);

/**
 * @brief Create a sprite background and fills the background buffer
 * 
 * @param xpm pixmap
 * @param enumtype with mode used
 * @return int 0 upon success and non-zero otherwise
 */
int create_sprite_background(xpm_map_t xpm,enum xpm_image_type enumtype);

/**
 * @brief Create a sprite leaderboard and fills the leaderboard buffer
 * 
 * @param xpm pixmap
 * @param enumtype with mode used
 * @return int 0 upon success and non-zero otherwise
 */
int create_sprite_leaderboard(xpm_map_t xpm,enum xpm_image_type enumtype);

/**
 * @brief Destroys and Frees the mem used with this Sprite
 * 
 * @param sp sprite to be destroyed
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Destroys and Frees the mem used with this Sprite
 * 
 * @param sp sprite to be destroyed
 */
void destroy_menu_sprite(MenuSprite * sp);

/**
 * @brief Draws sprite in the position given by the x and y elements
 * 
 * @param sp sprite to be draw
 * @return int 0 upon success and non-zero otherwise
 */
int draw_sprite(Sprite *sp);

/**
 * @brief Draws sprite in the position given by the x and y arguments
 * 
 * @param x position in horizontal axe
 * @param y position in vertical axe
 * @param sp sprite to be draw
 * @return int 0 upon success and non-zero otherwise
 */
int draw_menu_sprite(int x, int y, MenuSprite *sp);

/**
 * @brief Draws cursor sprite in the position given by the x and y elements
 * 
 * @param sp sprite to be draw
 * @return int 0 upon success and non-zero otherwise
 */
int draw_cursor_sprite(CursorSprite *sp);

/**
 * @brief moves the sprite upon the speed elements and updates positions
 * 
 * @param sp sprite to be moved
 * @param limitChange true if reached the limits of the screen else otherwise
 * @return int 0 upon success and non-zero otherwise
 */
int move_sprite(Sprite *sp,bool limitChange);

/**
 * @brief moves the cursor sprite upon the move of the mouse and updates positions
 * 
 * @param x horizontal displacement
 * @param y vertical displacement
 * @return int 0 upon success and non-zero otherwise
 */
int move_cursor(int x, int y);

/**
 * @brief Set the claw yspeed object
 * 
 * @param direction 0 if it doesn't move, 1 if moves down and -1 if goes up
 * @return int 0 upon success and non-zero otherwise 
 */
int set_claw_yspeed(int direction);

/**
 * @brief Set the claw2 yspeed object, used when in multiplayer mode
 * 
 * @param direction 0 if it doesn't move, 1 if moves down and -1 if goes up
 * @return int 0 upon success and non-zero otherwise 
 */
int set_claw2_yspeed(int direction);

/**
 * @brief updates claw speed and positions upon car moves
 * 
 * @return int 0 upon success and non-zero otherwise 
 */
int check_claw_changes();

/**
 * @brief updates claw2 speed and positions upon car2 moves, used when in multiplayer mode
 * 
 * @return int 0 upon success and non-zero otherwise 
 */
int check_claw2_changes();

/**
 * @brief Set the car speed object upon background move
 * 
 * @param direction 0 if it doesn't move, 1 if moves to the left and -1 if goes to the rights
 * @return int 0 upon success and non-zero otherwise 
 */
int set_car_speed(int direction);

/**
 * @brief Set the car2 speed object upon background move, used when in multiplayer mode
 * 
 * @param direction 0 if it doesn't move, 1 if moves to the left and -1 if goes to the rights
 * @return int 0 upon success and non-zero otherwise 
 */
int set_car2_speed(int direction);

/**
 * @brief Get the menu sprite correspondent to the number
 * 
 * @param number that indicates the sprite
 * @return MenuSprite* the sprite correspondent to the number
 */
MenuSprite* get_sprite_number(int number);

/**
 * @brief handles every move, animation and collisions of every sprite on game
 * 
 * @param state 0 if it hasn't any object in the claw and 1 if has catched an object
 * @param type 0 if it is an undefined object, 1 if it is a gold and 2 if it is a clock
 * @param idx 0 if it isn't a gold that was catched and if it is a gold corresponds to his index in the array of golds
 * @param score current score
 * @param clock_counter time left to the game to end
 * @param click if the left button was pressed
 */
void handle_sprite_changes(int state,int type, int idx,long long unsigned int * score, int * clock_counter,bool click);

/**
 * @brief draws every sprite active on the game screen
 * 
 * @param clock_counter time left to the game to end
 * @param score current score
 */
void draw_game_screen(int clock_counter, long long unsigned int score);

/**
 * @brief draws every sprite active on the menu screen
 *
 * @param click if left button is down
 * @param date current date
 * @return int 0 upon success and non-zero otherwise 
 */
int draw_menu_screen(bool click,Date_t * date);

/**
 * @brief draws every sprite active on the score screen
 * 
 * @param score score ponctuation
 */
void draw_score_screen(long long unsigned int score);

/**
 * @brief draws every sprite active on the multiplayer score screen 
 * 
 * @param score score ponctuation
 * @param score2 score ponctuation player 2
 */
void draw_multi_score_screen(long long unsigned int score, long long unsigned int score2);

/**
 * @brief draws every sprite active on the leaderboard screen
 * 
 * @param point - pointer to the first elemente of the points struct
 */
void draw_leaderboard_screen(struct Point * point);

/**
 * @brief loads and creates every sprite
 * 
 * @return int 0 upon success and non-zero otherwise 
 */
int load_sprites();

/**
 * @brief destroys every sprite ever created
 * 
 */
void destroy_sprites();

/**
 * @brief adds animation possibility to the sprite
 * 
 * @param sp the sprite that has new animation
 * @param xpm pixmap to the new animation
 * @param enumtype with mode used
 */
void add_animate_sprite(Sprite *sp, xpm_map_t xpm,enum xpm_image_type enumtype);

/**
 * @brief adds animation possibility to the cursor
 * 
 * @param sp the sprite that has new animation
 * @param xpm pixmap to the new animation
 * @param enumtype with mode used
 */
void add_animate_sprite_cursor(CursorSprite *sp, xpm_map_t xpm,enum xpm_image_type enumtype);

/**
 * @brief moves to the next animation on the vector of animations if the last goes back to the beginning
 * 
 * @param sp the sprite that is going to change animation
 */
void change_sprite_animation(Sprite *sp);

/**
 * @brief moves to the next animation on the vector of animations if the last goes back to the beginning
 * 
 */
void change_sprite_animation_cursor();

/**
 * @brief checks collision between a sprite with a square with width x_size and height y_size
 * 
 * @param x position in horizontal axe of the top left corner of the square
 * @param y position in horizontal axe of the top left corner of the square
 * @param x_size square with
 * @param y_size square height
 * @param sp sprite that is checking collision
 * @return int 0 if no collision happened, 1 parcial collision and 2 full collision
 */
int check_collision(int x, int y, int x_size, int y_size , Sprite * sp);

/**
 * @brief goes back to the initial positions and speed
 * 
 * @param seed to be set of or program
 */
void reset_sprites(bool useSeed);

/**
 * @brief handles every move, animation and collisions of every sprite on game
 * 
 * @param state 0 if it hasn't any object in the claw and 1 if has catched an object
 * @param type of the object that player 1 catched 0 if it is an undefined object, 1 if it is a gold and 2 if it is a clock
 * @param idx of the object that player 1 catched 0 if it isn't a gold that was catched and if it is a gold corresponds to his index in the array of golds
 * @param type2 of the object that player 1 catched 0 if it is an undefined object, 1 if it is a gold and 2 if it is a clock
 * @param idx2 of the object that player 1 catched 0 if it isn't a gold that was catched and if it is a gold corresponds to his index in the array of golds
 * @param score current player 1 score 
 * @param score2 current player 2 score 
 * @param clock_counter time left to player 1
 * @param clock_counter2 time left to player 2
 */
void handle_multiplayer_sprite_changes(int state,int type,int idx,int type2,int idx2,long long unsigned int* score,long long unsigned int* score2, int * clock_counter, int * clock_counter2);

/**
 * @brief draws every sprite active on the multiplayer game screen 
 * 
 * @param clock_counter time left to player 1
 * @param score player 1 score
 * @param clock_counter2 time left to player 2
 * @param score2 player 2 score
 */
void draw_multiplayer_game_screen(int clock_counter, long long unsigned int score,int clock_counter2, long long unsigned int score2);

/**
 * @brief used to write loading screen
 * 
 */
void draw_loading_screen();
