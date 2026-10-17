#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <lcom/lcf.h>
#include "sprites.h"
#include "video_card.h"
#include "proj_main_loop.h"
#include "mouse.h"
#include "kbd.h"

/**
 * @brief Defines the X-coordinate position of the car on the screen.
 */
#define SCREEN_CAR_POS_X 542 

/**
 * @brief Defines the Y-coordinate position of the car on the screen.
 */
#define SCREEN_CAR_POS_Y 413

/**
 * @brief Structure representing the pause menu with options.
 */
typedef struct{
    sprite options[3]; /**< Array of sprite options in the pause menu. */
} PauseMenu;

/**
 * @brief Structure representing the game elements.
 */
typedef struct{
    sprite background; /**< Background sprite of the game. */
    sprite car;  /**< Car sprite in the game. */
    sprite race;  /**< Race sprite in the game. */
    sprite stick; /**< Stick sprite in the game. */
    sprite stick_margin; /**< Stick margin sprite in the game. */
    sprite clock; /**< Clock sprite in the game. */
    mouse_sprite* mouse;  /**< Pointer to the mouse sprite used in the game. */
} Game;

/**
 * @brief Enumeration representing the current state of the game.
 */
typedef enum {PLAYING, /**< The game is currently being played. */
            PAUSE_MENU,  /**< The game is in the pause menu. */
            BEGIN, /**< The game is at the beginning state. */
            EXIT, /**< The game is in the exit state. */
            DELAY /**< The game is delaying before going to the menu. Used to show the victory image. */
            } CurrentState;

/**
 * @brief Enumeration representing the possible movements of the car.
 */
typedef enum {NO_MOVEMENT, /**< No movement of the car. */
                FORWARD, /**< The car is moving forward. */
                BACKWARD /**< The car is moving backward. */
                } Move;

/**
 * @brief Enumeration representing the stick movement states.
 */
typedef enum {STICK_MOVING, /**< The stick is moving. */
            STICK_NO_MOVEMENT,  /**< The stick is not moving. */
             RESET /**< The stick movement is reset. */
             } StickMove;

/**
 * @brief Cleans up the game, releasing any allocated resources.
 */
void (clean_game) ();

/**
 * @brief Initializes the game with the given mouse cursor and race ID.
 * 
 * @param cursor Pointer to the mouse_sprite structure representing the cursor.
 * @param race_id Identifier for the race track.
 * @return int Returns 0 on success, non-zero otherwise.
 */
int (initialize_game) (mouse_sprite* cursor, uint8_t race_id);

/**
 * @brief Controls the game state based on interrupts from the timer, mouse, and keyboard.
 * 
 * @param timer_irq The IRQ line for the timer.
 * @param mouse_irq The IRQ line for the mouse.
 * @param kbd_irq The IRQ line for the keyboard.
 * @param rtc_irq The IRQ line for the rtc.
 * @param cursor Pointer to the mouse_sprite structure representing the cursor.
 * @param race_track Identifier for the race track.
 * @return Game_State The current state of the game.
 */
Game_State (game_controller) (int timer_irq, int mouse_irq, int kbd_irq, int rtc_irq, mouse_sprite* cursor,uint8_t race_track);

/**
 * @brief Fills an array with number sprites.
 */
void (fill_numbers_array)();

/**
 * @brief Draws the game view on the screen.
 * 
 * @return int Returns 0 on success, non-zero otherwise.
 */
int (game_viewer_draw) ();

/**
 * @brief Moves the car sprite.
 */
void (move_car) ();

/**
 * @brief Controls the stick movement.
 */
void (control_stick) ();

/**
 * @brief Checks for a collision between the mouse cursor and an object.
 * 
 * @param mouse Pointer to the mouse_sprite structure representing the cursor.
 * @param object Pointer to the sprite structure representing the object.
 * @return bool Returns true if a collision is detected, false otherwise.
 */
bool (check_colision) (mouse_sprite* mouse, sprite* object);

/**
 * @brief Calculates the distance between two coordinates.
 * 
 * @param x1 X-coordinate of the first point.
 * @param y1 Y-coordinate of the first point.
 * @param x2 X-coordinate of the second point.
 * @param y2 Y-coordinate of the second point.
 * @return double The distance between the two points.
 */
double (distance_coordinates) (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief Updates the stick coordinates based on the distance between two points.
 * 
 * @param x1 X-coordinate of the first point.
 * @param y1 Y-coordinate of the first point.
 * @param x2 X-coordinate of the second point.
 * @param y2 Y-coordinate of the second point.
 * @param dist The distance between the two points.
 */
void (update_stick_coordinates) (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, double dist);

/**
 * @brief Stores the performance data.
 */
void (store_performance) ();

/**
 * @brief Loads the pause menu.
 */
void (load_pause_menu) ();

/**
 * @brief Draws the pause menu on the screen.
 */
void (pause_menu_draw) ();

/**
 * @brief Controls the pause menu and returns the current state.
 * 
 * @return CurrentState The current state after handling the pause menu.
 */
CurrentState (pause_menu_controller) ();

/**
 * @brief Resets all game variables to their initial states.
 */
void (reset_all_variables) ();

#endif
