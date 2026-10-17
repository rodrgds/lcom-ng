#pragma once

#include "../gpu/gpu.h"
#include "../kbd_mouse/kbd.h"
#include "../kbd_mouse/mouse.h"
#include "../rtc/rtc.h"
#include "../serialport/serialport.h"
#include "model.h"
#include "viewer.h"
#include "xpms.h"
#include <lcom/lcf.h>

#ifdef DEBUG
#  define DEBUG_MSG(...) printf("[DEBUG] %s:%d: ", __FILE__, __LINE__); \
                         printf(__VA_ARGS__); \
                         printf("\n");
#else
#  define DEBUG_MSG(...)
#endif

#define REFRESH_RATE (60 / 30)


/**
 * @brief I/O Devices used
 *
 */
enum devices {
  TIMER,     /*!< @brief Timer */
  KEYBOARD,  /*!< @brief Keyboard */
  MOUSE,     /*!< @brief Mouse */
  RTC,       /*!< @brief RTC */
  SERIALPORT /*!< @brief Serial port */
};

/**
 * @brief Enumerates the game states
 *
 */
typedef enum game_states {
  MENU,               /*!< @brief Main menu state */
  SINGLE_PLAYER_GAME, /*!< @brief 1 player game state */
  MULTI_PLAYER_GAME,  /*!< @brief 2 player game state */
  WAITING_CONNECTION, /*!< @brief Waiting connection state */
  CREDITS,            /*!< @brief Credits state */
  GAME_OVER,          /*!< @brief Game over state */
  GAME_WIN,           /*!< @brief Game win state */
  BLOCKED,             /*!< @brief Blocked (can not play) state */
  NONE_STATE,         /*!< @brief None state */
  EXIT,               /*!< @brief Exit state */
} game_state_t;

/**
 * @brief Struct that represents a game state
 *
 * @param state Current game state
 * @param controller Pointer to the controller of the current state
 */
struct state {
  game_state_t state;             /*!< @brief Current Game state */
  void (*controller)(int device); /*!< @brief Pointer to the controller of the current state */
};

/**
 * @brief Array of game states
 */
struct state game_states_list[11];

/**
 * @brief Initializes the game states
 *
 * @param device Device that signaled the interrupt
 */
void(menuController)(int device);

/**
 * @brief Controller of the single player game state
 * 
 * @param device Device that signaled the interrupt 
 */
void(singlePlayerController)(int device);

/**
 * @brief Controller of the waiting connection state
 * 
 * @param device Device that signaled the interrupt
 */
void(waitingConnectionController)(int device);

/**
 * @brief Controller of the multi player game state
 * 
 * @param device Device that signaled the interrupt
 */
void(multiPlayerController)(int device);

/**
 * @brief Controller of the credits state
 * 
 * @param device Device that signaled the interrupt
 */
void(creditsController)(int device);

/**
 * @brief Controller of the game over state
 * 
 * @param device Device that signaled the interrupt
 */
void(gameOverController)(int device);

/**
 * @brief Controller of the blocked state
 * 
 * @param device Device that signaled the interrupt
 */
void(blockedController)(int device);


/** 
* @brief Controller of the game win state
*
* @param device Device that signaled the interrupt
*/
void(gameOverController)(int device);

/**
* @brief Controller of the game win state
*
* @param device Device that signaled the interrupt
*/
void(gameWinController)(int device);
