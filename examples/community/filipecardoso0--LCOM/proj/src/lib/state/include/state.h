#ifndef _STATE_H_
#define _STATE_H_

#include "game_state.h"
#include "menu_state.h"
#include "action.h"
#include "../../../drivers/mouse/include/mouse.h"

enum app_state{MENU, GAME, SNULL};

typedef enum app_state app_state_t;

/**
 * @brief Interface to access different states
 *        Why not putting them all in this .h?
 *          - This way we keep everything decoupled and organized:
 *              * Allows us to focus on individual modules
 *              * Parts of the program can only access what they need
 *                  i.e snake.h and snake_ctrl can interact with snake_state.h
 *                  directly without loading unecessary data
 *              * Keeps system flexible
 *              * Makes debugging easier
 *          - Having a common interface delegating tasks makes circular
 *            includes less probable to occur
 */


/**
 * @brief Returns the current state of the app
 * 
 * @return app_state_t the app state
 */
app_state_t get_app_state();

/**
 * @brief Set's the app's state to MENU
 * 
 * @return int 0 upon success, 1 if app is already set to MENU
 */
int set_app_state_menu();

/**
 * @brief Set's the app's state to GAME
 * 
 * @return int 0 upon success, 1 if app is already set to GAME
 */
int set_app_state_game();


/**
 * @brief Set's the app's state to SNULL
 * 
 * @return int 0 upon success, 1 if app is already set to SNULL
 */
int set_app_state_null();

/**
 * @brief Updates the game state or the menu according to the app's state.
 * 
 */
void state_step();
/**
 * @brief Set's the action's state to the input value.
 * 
 * @param action action_t that contains the new state of the action.
 */
void state_set_action(action_t action);

/**
 * @brief Returns the state of an action.
 * 
 * @return action_t the action's state.
 */
action_t state_get_action();

struct packet* state_get_packet();

void state_set_packet(struct packet* packet);

#endif
