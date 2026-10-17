#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <math.h>
#include <stdint.h>

#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "drivers/timer/i8254.h"
#include "drivers/video/gpu.h"
#include "model/level/level.h"
#include "model/menu/game_over.h"
#include "model/menu/main_menu.h"
#include "model/menu/victory.h"
#include "model/utils/cursor.h"
#include "model/utils/keys.h"
#include "model/utils/sprite.h"

extern uint8_t scan_code; /**< Keyboard scan code */
extern struct packet pp;  /**< Mouse packet */
extern int frame_counter; /**< Frame counter for the game */
extern uint16_t x_res;    /**< X resolution of the screen */
extern uint16_t y_res;    /**< Y resolution of the screen */

/**
 * @brief Represents a single Game instance.
 *
 * The Game struct is opaque.
 */
typedef struct Game Game;

/**
 * @brief Possible states of the game.
 */
typedef enum {
    MENU,      /**< Main menu state */
    LEVEL,     /**< Active game level state */
    VICTORY,   /**< Game victory state */
    GAME_OVER, /**< Game over state */
    EXIT       /**< Exit state */
} State;

/**
 * @brief Creates and initializes a new Game instance.
 *
 * @return Pointer to the created Game structure.
 */
Game *create_game();

/**
 * @brief Frees the memory associated with the Game instance.
 *
 * @param game Pointer to the Game structure to destroy.
 */
void destroy_game(Game *game);

/**
 * @brief Handles timer interrupts and updates game logic accordingly.
 *
 * @param game Pointer to the Game structure.
 */
void game_timer_handler(Game *game);

/**
 * @brief Handles keyboard input events and updates game state.
 *
 * @param game Pointer to the Game structure.
 */
void game_keyboard_handler(Game *game);

/**
 * @brief Handles mouse input events and updates game state.
 *
 * @param game Pointer to the Game structure.
 */
void game_mouse_handler(Game *game);

/**
 * @brief Returns the current state of the game.
 *
 * @param game Pointer to the Game structure.
 * @return Current game state.
 */
State get_state(Game *game);

#endif /* GAME_H */
