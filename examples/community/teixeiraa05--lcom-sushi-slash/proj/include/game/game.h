#ifndef __GAME_H
#define __GAME_H

#include <stdbool.h>

#include "boss.h"
#include "sushi.h"
#include "menu_state.h"
#include "pause_menu_state.h"
#include "game_over.h"
#include "sprite.h"
#include "waves.h"
#include "slash_combo.h"
#include "../init.h"
#include "../../../lab4/mouse.h"

/**
 * @file game.h
 * @brief Declares the public game state and gameplay lifecycle API.
 */

/** @brief Initial number of player lives in a gameplay session. */
#define LIVES 3

/** @brief Maximum number of active gameplay items on screen. */
#define MAX_ACTIVE_SUSHI 8

/** @brief Maximum number of active cut fragments on screen. */
#define MAX_ACTIVE_FRAGMENTS 16

/** @brief Score threshold at which easy-mid difficulty starts. */
#define GAME_EASY_MID_SCORE 15

/** @brief Score threshold at which mid difficulty starts. */
#define GAME_MID_SCORE 30

/** @brief Score threshold at which mid-hard difficulty starts. */
#define GAME_MID_HARD_SCORE 45

/** @brief Score threshold at which hard difficulty starts. */
#define GAME_HARD_SCORE 100

/** @brief First score milestone that starts a boss encounter. */
#define GAME_BOSS_FIRST_SCORE 50

/** @brief Score gap between boss encounter milestones. */
#define GAME_BOSS_SCORE_STEP 50

/** @brief Number of keys in the first boss sequence. */
#define GAME_BOSS_FIRST_SEQUENCE_LENGTH 5

/** @brief Number of extra keys added by each later boss. */
#define GAME_BOSS_SEQUENCE_STEP 2

/** @brief Duration of the boss reward popup, in gameplay ticks. */
#define GAME_BOSS_REWARD_POPUP_TICKS 60

/**
 * @brief High-level state of the application.
 */
typedef enum {
  /** @brief Main menu is active. */
  MENU,

  /** @brief Singleplayer gameplay is active. */
  GAME,

  /** @brief Singleplayer mode selection/state marker. */
  SINGLEPLAYER,

  /** @brief Multiplayer mode selection/state marker. */
  MULTIPLAYER,

  /** @brief Gameplay is paused. */
  PAUSED,

  /** @brief Game is waiting for another event or player. */
  WAITING,

  /** @brief Game-over screen is active. */
  GAME_OVER,

  /** @brief Application should exit. */
  EXIT
} GameState;

/**
 * @brief Device categories used by the game.
 */
typedef enum {
  /** @brief Keyboard input device. */
  KEYBOARD,

  /** @brief Mouse input device. */
  MOUSE,

  /** @brief Periodic timer device. */
  TIMER,

  /** @brief Real-time clock device. */
  RTC, 

  /** @brief Graphics card/device. */
  GRAPHICS,

  /** @brief Serial port device. */
  SERIALPORT
} Device;

/**
 * @brief Runtime data for a singleplayer gameplay session.
 */
typedef struct {
    /** @brief Active regular sushi and bomb items. */
    sushi_t active_sushi[MAX_ACTIVE_SUSHI];

    /** @brief Active fragments created by cut regular sushi. */
    sushi_fragment_t active_fragments[MAX_ACTIVE_FRAGMENTS];

    /** @brief Mutable wave sequence used by the current difficulty. */
    wave_t waves[WAVE_PATTERN_COUNT];

    /** @brief Wave scheduler runtime state. */
    wave_runtime_t wave_runtime;

    /** @brief Current difficulty level. */
    difficulty_t current_difficulty;

    /** @brief Number of active entries in active_sushi. */
    uint8_t num_active_sushi;

    /** @brief Number of active entries in active_fragments. */
    uint8_t num_active_fragments;

    /** @brief Quick-slash combo runtime state and popup data. */
    slash_combo_t slash_combo;

    /** @brief Session statistics accumulated for the game-over screen. */
    game_over_stats_t stats;

    /** @brief Currently active boss wave, if boss_active is true. */
    boss_t active_boss;

    /** @brief Whether a boss wave is currently active during gameplay. */
    bool boss_active;

    /** @brief Next displayed score milestone that should spawn a boss. */
    uint32_t next_boss_score;

    /** @brief Number of boss encounters started in this session. */
    uint32_t boss_count;

    /** @brief Points shown by the pending boss reward popup. */
    uint32_t pending_boss_score_popup;

    /** @brief Remaining ticks for the boss reward popup. */
    uint8_t boss_score_popup_ticks;

    /** @brief Active seed used for random generation. */
    uint32_t current_seed;

    /** @brief Deterministic PRNG seed for sushi and wave generation. */
    uint32_t sushi_prng_seed;

    /** @brief Deterministic PRNG seed for boss key and position generation. */
    uint32_t boss_prng_seed;
} gameplay_data_t;

/**
 * @brief Main game state container shared by menus and gameplay modes.
 */
typedef struct {
    /** @brief Current high-level application state. */
    GameState state;

    /** @brief Current player score. */
    uint32_t score;

    /** @brief Current remaining player lives. */
    uint8_t lives;

    /** @brief Gameplay background sprite. */
    sprite_t background;

    /** @brief Singleplayer gameplay runtime data. */
    gameplay_data_t singleplayer;

    /** @brief State-specific data for menus and multiplayer screens. */
    union {
        /** @brief Main menu runtime state. */
        menu_t menu;

        /** @brief Pause menu runtime state. */
        pause_menu_t pause_menu;
        
        /** @brief Game-over screen runtime state. */
        game_over_t game_over;

        /** @brief Multiplayer runtime state. */
        struct {
          /** @brief Active multiplayer sushi and bomb items. */
          sushi_t active_sushi[MAX_ACTIVE_SUSHI];

          /** @brief Active multiplayer cut fragments. */
          sushi_fragment_t active_fragments[MAX_ACTIVE_FRAGMENTS];

          /** @brief Number of active multiplayer sushi entries. */
          uint8_t num_active_sushi;

          /** @brief Number of active multiplayer fragment entries. */
          uint8_t num_active_fragments;

          /** @brief Current score for player 1. */
          uint32_t player1_score;

          /** @brief Current score for player 2. */
          uint32_t player2_score;
        } multiplayer;
    } data;
} GameInfo;

/**
 * @brief Destroy active gameplay objects and reset active object counts.
 *
 * @param game Game state to clear.
 */
void gameplay_clear(GameInfo *game);

/**
 * @brief Initialize a new singleplayer gameplay session.
 *
 * @param game Game state to initialize.
 */
void gameplay_start(GameInfo *game);

/**
 * @brief Advance singleplayer gameplay by one tick.
 *
 * Handles wave spawns, item physics, missed items, fragments, difficulty
 * changes, and game-over transitions.
 *
 * @param game Game state to update.
 * @param current_tick Current gameplay/session tick.
 * @param last_spawn_tick Output storage updated when a wave emits a spawn.
 */
void gameplay_update(GameInfo *game, int current_tick, int *last_spawn_tick);

/**
 * @brief Draw active gameplay items, fragments, and UI effects.
 *
 * @param game Game state to draw.
 * @param assets Shared assets used for drawing UI effects.
 * @param current_tick Current gameplay/session tick used for boss UI timers.
 */
void gameplay_draw(const GameInfo *game, const Assets *assets, uint32_t current_tick);

/**
 * @brief Process the current mouse slash against active gameplay items.
 *
 * Regular sushi is cut into fragments and awards score. Bombs immediately end
 * the gameplay session when hit.
 *
 * @param game Game state to mutate.
 * @param mouse_state Current mouse gameplay input state.
 * @param current_tick Current gameplay/session tick.
 */
void gameplay_handle_cut(GameInfo *game, MouseGameState *mouse_state, uint32_t current_tick);

/**
 * @brief Process one keyboard character against the active boss sequence.
 *
 * @param game Game state to mutate.
 * @param input Uppercase or lowercase input character.
 * @param current_tick Current gameplay/session tick.
 */
void gameplay_handle_boss_key(GameInfo *game, char input, uint32_t current_tick);

/**
 * @brief Check whether a boss wave is currently active.
 *
 * @param game Game state to inspect.
 * @return true if a boss wave is active, false otherwise.
 */
bool gameplay_boss_active(const GameInfo *game);

/**
 * @brief Get the current sushi PRNG seed.
 * @param game Game state to inspect.
 * @return The sushi PRNG seed value.
 */
uint32_t gameplay_get_sushi_seed(const GameInfo *game);

/**
 * @brief Set the sushi PRNG seed.
 * @param game Game state to mutate.
 * @param seed The new seed value.
 */
void gameplay_set_sushi_seed(GameInfo *game, uint32_t seed);

/**
 * @brief Get the current boss PRNG seed.
 * @param game Game state to inspect.
 * @return The boss PRNG seed value.
 */
uint32_t gameplay_get_boss_seed(const GameInfo *game);

/**
 * @brief Set the boss PRNG seed.
 * @param game Game state to mutate.
 * @param seed The new seed value.
 */
void gameplay_set_boss_seed(GameInfo *game, uint32_t seed);

#endif /* __GAME_H */
