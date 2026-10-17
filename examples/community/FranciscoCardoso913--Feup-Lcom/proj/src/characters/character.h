#ifndef __CHARACTER_H
#define __CHARACTER_H

#include <lcom/lcf.h>

#include "../drivers/graphics/graphics.h"
#include "../sprites/falling_block.h"
#include "../sprites/sprite.h"
#include "../maps/map.h"

#include "../xpm/zeldas/fireZelda/running_down_shield0.xpm"
#include "../xpm/zeldas/fireZelda/shield_left/left_shield1.xpm"
#include "../xpm/zeldas/fireZelda/shield_left/left_shield2.xpm"
#include "../xpm/zeldas/fireZelda/shield_left/left_shield3.xpm"
#include "../xpm/zeldas/fireZelda/shield_left/left_shield4.xpm"
#include "../xpm/zeldas/fireZelda/shield_left/left_shield5.xpm"
#include "../xpm/zeldas/fireZelda/shield_left/left_shield6.xpm"
#include "../xpm/zeldas/fireZelda/shield_right/shield_right1.xpm"
#include "../xpm/zeldas/fireZelda/shield_right/shield_right2.xpm"
#include "../xpm/zeldas/fireZelda/shield_right/shield_right3.xpm"
#include "../xpm/zeldas/fireZelda/shield_right/shield_right4.xpm"
#include "../xpm/zeldas/fireZelda/shield_right/shield_right5.xpm"
#include "../xpm/zeldas/fireZelda/shield_right/shield_right6.xpm"
#include "../xpm/zeldas/waterZelda/running_down_shield0.xpm"
#include "../xpm/zeldas/waterZelda/shield_left/left_shield1.xpm"
#include "../xpm/zeldas/waterZelda/shield_left/left_shield2.xpm"
#include "../xpm/zeldas/waterZelda/shield_left/left_shield3.xpm"
#include "../xpm/zeldas/waterZelda/shield_left/left_shield4.xpm"
#include "../xpm/zeldas/waterZelda/shield_left/left_shield5.xpm"
#include "../xpm/zeldas/waterZelda/shield_left/left_shield6.xpm"
#include "../xpm/zeldas/waterZelda/shield_right/shield_right1.xpm"
#include "../xpm/zeldas/waterZelda/shield_right/shield_right2.xpm"
#include "../xpm/zeldas/waterZelda/shield_right/shield_right3.xpm"
#include "../xpm/zeldas/waterZelda/shield_right/shield_right4.xpm"
#include "../xpm/zeldas/waterZelda/shield_right/shield_right5.xpm"
#include "../xpm/zeldas/waterZelda/shield_right/shield_right6.xpm"


enum Direction { LEFT, RIGHT, UP, DEFAULT };
enum Elements { FIRE, WATER };
#define GRAVITY 1 /** @brief Default vertical acelarition to when Objects are falling*/
#define DEFAULT_SPEED 7 /** @brief Default horizontal speed when characters are moving*/
#define JUMP 15 /** @brief vertical speed to when a character jump*/
#define CHECKBOX_PADDING 10 /** @brief Margin of the sprite which is not going to be considered as a checkbox*/

typedef struct {
    enum Direction direction;
    int animation_delay;
    int frames_to_next_change;
    int current_sprite;
    Sprite* sprite;
    uint32_t* left[6];
    uint32_t* right[6];
    uint32_t* front;
    enum Elements element;
} Character;


Character* fireboy;
Character* watergirl;
extern Map* current_map;


/**
 * @brief Checks if block hit the ground
 * 
 * @param block block to check
 * @return int 1 if block has it the ground, 0 otherwise
 */
int hit_ground(Sprite* block);

/**
 * @brief Moves blocks that are falling, stops all blocks that have hit the ground and draws all the blocks
 * 
 */
void draw_falling_blocks();

/**
 * @brief Erases all the blocks from the screen
 * 
 */
void erase_blocks();



/**
 * @brief Checks if there is a wall below the character.
 * 
 * @param character The character to analyze.
 * @return int Returns 1 if there is a wall below the character, 0 otherwise.
 */
int wall_down(Character* character);

/**
 * @brief Checks if there is a wall to the left of the character.
 * 
 * @param character The character to analyze.
 * @return int Returns 1 if there is a wall to the left of the character, 0 otherwise.
 */
int wall_left(Character* character);

/**
 * @brief Checks if there is a wall to the right of the character.
 * 
 * @param character The character to analyze.
 * @return int Returns 1 if there is a wall to the right of the character, 0 otherwise.
 */
int wall_right(Character* character);

/**
 * @brief Checks if there is a wall above the character.
 * 
 * @param character The character to analyze.
 * @return int Returns 1 if there is a wall above the character, 0 otherwise.
 */
int wall_up(Character* character);

/**
 * @brief Checks if the character is in front of the fire door.
 * 
 * @param character The character to analyze.
 * @return int Returns 1 if the character is in front of the fire door, 0 otherwise.
 */
int door_fire(Character* character);

/**
 * @brief Checks if the character is in front of the water door.
 * 
 * @param character The character to analyze.
 * @return int Returns 1 if the character is in front of the water door, 0 otherwise.
 */
int door_water(Character* character);

/**
 * @brief Draws the character.
 * 
 * @param character The character to be drawn.
 * @return int Returns 1 if an error occurs while drawing, 0 otherwise.
 */
int draw_character(Character* character);

/**
 * @brief Creates the characters.
 */
int create_characters();

/**
 * @brief Sets the character's position.
 * 
 * @param character The character to change the position.
 * @param x The new x position of the character.
 * @param y The new y position of the character.
 */
void set_position(Character* character, int x, int y);

/**
 * @brief Updates the current character's sprite.
 * 
 * @param character The character to be analyzed.
 */
void character_current_sprite(Character* character);

/**
 * @brief Checks if the character is on the ground.
 * 
 * @param character The character to be analyzed.
 * @return int Returns 1 if the character is on the ground, 0 otherwise.
 */
int is_on_ground(Character* character);

/**
 * @brief Updates the sprite values of the character.
 * 
 * @param character The character to be updated.
 */
void update_character(Character* character);

/**
 * @brief Changes the character's direction.
 * 
 * @param character The character to change the direction.
 * @param dir The new direction of the character.
 * @return int Returns 1 if the new position is different from the previous position, 0 otherwise.
 */
int update_direction(Character* character, enum Direction dir);

/**
 * @brief Moves the character.
 * 
 * @param character The character to move.
 */
void move(Character* character);

/**
 * @brief Moves the character to the left.
 * 
 * @param character The character to be moved.
 */
void move_left(Character* character);

/**
 * @brief Moves the character to the right.
 * 
 * @param character The character to be moved.
 */
void move_right(Character* character);

/**
 * @brief Moves the character up.
 * 
 * @param character The character

 to move.
 */
void jump(Character* character);

/**
 * @brief Makes the character stop moving.
 * 
 * @param character The character to stop.
 */
void stop_moving(Character* character);

/**
 * @brief Checks if the character is on fire.
 * 
 * @param character The character to be analyzed.
 * @return int Returns 1 if the character is on fire, 0 otherwise.
 */
int on_fire(Character* character);

/**
 * @brief Checks if the character is on water.
 * 
 * @param character The character to be analyzed.
 * @return int Returns 1 if the character is on water, 0 otherwise.
 */
int on_water(Character* character);

/**
 * @brief Checks if the character is on the poison
 * 
 * @param character The character to be analyzed
 * @return int Returns 1 if the character is on poison, 0 otherwise
 */
int on_poison(Character* character);
#endif /* __CHARACTER_H */
