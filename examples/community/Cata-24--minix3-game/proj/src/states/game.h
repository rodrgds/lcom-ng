/**
 * @file game.h
 * @brief Game state functions related to the drawing of the game, loading, keyboard processing and mouse related to this state
 * 
 */
#ifndef _GAME_STATE_
#define _GAME_STATE_

#include <lcom/lcf.h>
#include "../helper/structs.h"
#include "../drivers/video.h"
#include "../drivers/mouse.h"

/**
 * @brief Loads the sprites needed to initialize the game state and also sets their position
 * for the beginning of a new game
 */
int (load_game_state)();
/**
 * @brief Loads the sprites need for a specific level of the game and their respective position where needed
 */
int (load_game_level)();
/**
 * @brief Draws the sprites that were loaded according to the level and processes the timer logic for
 * movement and animation.
 */
int (draw_game_state)();
/**
 * @brief Processes the keyboard logic of the game, key.presses, direction changes and speed.
 */
int (kbd_game_ih)();
/**
 * @brief Processes the mouse logic of the game, direction in wich the player is looking at, clicks interaction and knife logic
 */
int (mouse_game_ih)(); // handle ao mouse e às consequências no jogo
/**
 * @brief Verifies the position of the mouse and generates an angle based on the player to get the direction to look at
 */
void (check_mouse_pos)();
/**
 * @brief Processes the new mouse position based on the packet received by the general mouse_ih
 */
void (set_mouse_pos)();
/**
 * @brief Function that decides the direction that the monster will take
 * If it's close to the player, directs towards him. If not, chooses a random direction
 * 
 * @param monster double pointer to the sprite pointer that stores the monster to be displayed
 * @param up monster sprite facing up
 * @param down monster sprite facing down
 * @param right monster sprite facing right
 * @param ddown monster sprite facing the diagonal down
 * @param dup monster sprite facing the diagonal up
 * @param reverse pointer to the reverse value of the monster
 */
void move_monster(struct Sprite **monster, struct Sprite *up, struct Sprite *down, struct Sprite *right, struct Sprite *ddown, struct Sprite *dup, bool *reverse);
/**
 * @brief Redirects to the move_monster for each of the 4 monsters with their respective values
 */
void update_monsters();
/**
 * @brief Processes the throwable knives depending on the direction that the player is facing
 */
void attack();
/**
 * @brief Function that verifies the colisions of the player with a specific sprite
 * 
 * @param element sprite to check the colision with the player
 * @return true if the sprite colides with player
 * @return false if the sprite does not colide with the player
 */
bool check_col(struct Sprite *element);
/**
 * @brief Verifies if a certain position is valid for the player to move in
 * 
 * @param x coordinate to be evaluated
 * @param y coordinate to be evaluated
 * @return int = 0 if possible to move and 1 otherwise.
 */
int check_player_col(int x, int y);
/**
 * @brief Verifies if a certain position is valid for the knife to move in
 * 
 * @param x coordinate to be evaluated
 * @param y coordinate to be evaluated
 * @return int = 0 if possible to move and 1 otherwise.
 */
int check_knife_col(int x, int y);
/**
 * @brief Processes the movement logic according to a sprite speed
 * 
 * @param moved Pointer to the sprite to be moved
 * @param pixs number of pixels to be moved
 * @param i position on the mov_sprites[] array
 * @return int = 0 if the function executed as expected and 1 otherwise.
 */
int change_pos(struct Sprite **moved, int pixs, int i);
/**
 * @brief Processes the stamina and speed logic and also iterates the mov_sprites array (contains the sprites that may be movable) and processes their movement regarding their speed atribute
 * 
 * @return int = 0 if the function executed as expected and 1 otherwise.
 */
int move_sprites();
/**
 * @brief Receives a key between pressing WASD or releasing WASD and changes the players direction and speed in respect to the key inputs
 * 
 * @param key key to be processed
 */
void process_keys(enum keys key);
/**
 * @brief Changes the player sprite to be shown (between the 8 options)
 * 
 * @param new sprite that will be shown as the player
 * @param new_reversed if the new sprite should be reversed
 */
void change_player(struct Sprite **new, bool new_reversed);
/**
 * @brief Change between the normal player sprite set and the blue player (speed) sprite set
 * 
 */
void change_ninja();
/**
 * @brief Checks if the player is near a bomb
 * 
 * @param i is the position of the bomb in the bomb array
 * @return true if he is near
 * @return false if he is not near
 */
bool is_near_bomb(int i);

#endif // _GAME_STATE_
