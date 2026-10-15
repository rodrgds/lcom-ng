#ifndef PLAYER_H
#define PLAYER_H

#include "../macros/includes.h"
#include "../load/view.h"

#include "../Items/package.h"


/**
 * @brief Changes all player movement in every direction to STILL, making him not move
 */
void reset_movement();


/**
 * @brief knows if player is holding down the letter "i" on the keyboard or not
 * @return Returns true if player is holding down "i" and false otherwise
 * 
 */
bool get_is_holding_i();

/**
 * @brief Gets the value of variable that registers if selected item on the inventory is equal to the one currently throwing
 * @return Returns variable that registers the index of inventory which item is being thrown
 */
int get_throwing_index();

/**
 * @brief Gets the value of the inventory index currently being used
 * @return Returns the value of the index of inventory currently being used
 */
int get_current_index();

/**
 * @brief Changes the variable taht registers the index of item of inventory being thrown to a value passed by argument
 * @param throw_timer Value to change variable that registers the index of item of inventory being thrown into
 */
void set_throwing_index(int throw_timer);

/**
 * @brief Controlls the action that the player entity can do in the game
 * @param scancode Value that will determine which action the player takes
 */
void player_controller(uint32_t scancode);

/**
 * @brief Moves the player accordingly to the keyboard inputs he has submitted
 */
void move_player();

/**
 * @brief Calls drawing function to draw the player taking into account the direction he faces
 */
void draw_player();
/**
 * @brief Eliminates all worker enemies that are in contact with the player character
 */
void kill_workers();

/**
 * @brief Removes package from the ground and places on character's inventory if character's inventory is not yet full and if character is in contact with said package
 * @param character Character that is removing the package from the board
 */
void remove_package(img_t character);

/**
 * @brief Removes item from the ground and places on character's inventory if character's inventory is not yet full and if character is in contact with said item
 * @param character Character that is removing the item from the board
 */
void remove_item_from_ground(img_t character);
/**
 * @brief Removes item from inventory and places it on the board
 * @param index Index position of inventory of item being removed from it
 */
void remove_from_inventory(int index);

/**
 * @brief Determines whether there is space on the inventory or not
 * @return Returns -1 if there isn't and if there is it returns the inventory index of the nearest free place
 */
int is_inventory_full();

/**
 * @brief Checks if a character is colliding with a teleport
 * @param character Character that is being check for collision with teleport
 * @return Returns -1 if there is no collision and Returns the index of teleport being collided if there is collision
 */
int is_on_teleport(img_t character);




#endif

