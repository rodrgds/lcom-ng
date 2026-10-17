/**
 * @file game.h
 * @brief Module for handling game loop.
 */

#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_


#include <lcom/lcf.h>

// Used Devices
#include <lcom/timer.h>
#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "devices/videocard.h"
#include "devices/RTC.h"

// Game Elements
#include "gameElements/playerShip.h"
#include "gameElements/enemy.h"
#include "gameElements/fire.h"
#include "gameElements/enemyFire.h"
#include "gameElements/spaceship_explosion.h"
#include "gameElements/enemyExplosion.h"
#include "gameElements/menu.h"
#include "gameElements/cursor.h"


//Sprites
#include "sprites/sprite.h"
#include "sprites/projPixmap.h"


/** @defgroup game Game
 * @{
 *
 * @brief game related functions
 */


/**
 * 
 * @brief Enum type for the game states
 * 
 */
typedef enum {
    MAINMENU, /**< Main menu state */
    INSTRUCTIONSMENU, /**< Leaderboard menu state */
    LEADERBOARDMENU, /**< Leaderboard menu state */
    PLAYING, /**< Playing state */
    ENDSCREEN, /**< End screen state */
    EXITED /**< Exited state */
} gameState;


/**
 * @brief Struct representing a game level.
 */
typedef struct {
    int maxTime;
    int enemySpawnRate;
} Level;

/**
 * @brief Struct representing the game state.
 */
typedef struct {
    gameState currentState;
    int currentLevel;
    int levelCountdown;
} Game;


// Functions to help handle device interrupts ----------------------------------------------------------

/**
 * @brief Handles key press events.
 */
void handle_pressed_key();  //Keyboard

/**
 * @brief Handles mouse movement events.
 */
void handle_mouse_move();  //Mouse

/**
 * @brief Handles the game loop, which updates game state based on timer interrupts.
 */
void handle_game_loop();  //Timer 0


//Functions to draw the game screens, menus and information --------------------------------------------
/**
 * @brief Displays the current screen based on the game state.
 */
void show_current_screen();

/**
 * @brief Draws the game screen during gameplay.
 */
void draw_game_screen();

/**
 * @brief Draws the game status including score, lives, and level.
 */
void draw_game_status();

/**
 * @brief Draws the end screen when the game is over.
 */
void draw_end_screen();


//Functions to prepare the game to be played and alter the game attributes ----------------------------
/**
 * @brief Resets the game to its initial state.
 */
void reset_game();

/**
 * @brief Initializes a level based on the level index.
 * 
 * @param levelIndex Index of the level to initialize.
 */
void init_level(int levelIndex);

/**
 * @brief Spawns an enemy at a random position.
 */
void spawn_enemy();

/** @} end of game */

#endif /* _LCOM_GAME_H_ */







