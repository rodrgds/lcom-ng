/**
 * @file state.h
 * @brief Game state machine and application entry point.
 */
#ifndef _STATE_H_
#define _STATE_H_

/**
 * @brief Application-level game states.
 */
typedef enum {
    START_SCREEN,    /**< Animated start/splash screen. */
    MAIN_MENU,       /**< Main menu. */
    TYPING_TEST,     /**< Fast Teclas typing game. */
    FAST_TECLAS_END, /**< End screen after a fast teclas round. */
    CONFIG,          /**< Fast teclas configuration screen. */
    SURVIVAL_CONFIG, /**< Survival mode configuration screen. */
    DINO_GAME,       /**< Dino mini-game. */
    SURVIVAL_GAME,   /**< Survival typing mode. */
    LEADERBOARD,     /**< Leaderboard screen. */
    GAME_OVER,       /**< Game over screen. */
    EXIT             /**< Exit the application. */
} GameState;

/**
 * @brief Initialize and run the main game loop.
 *
 * @return 0 on clean exit, non-zero on error.
 */
int start_game_loop();

#endif