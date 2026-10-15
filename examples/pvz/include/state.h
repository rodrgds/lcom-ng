#ifndef _STATE_H_
#define _STATE_H_

#include <stdbool.h>

/* -------------------------------------------------------
 *  Game State Machine
 *
 *  MAIN_MENU --[ENTER]---> PLAYING
 *            --[ESC]-----> QUIT
 *
 *  PLAYING   --[ESC]-----> PAUSED
 *            --[zombie crosses without lawnmower]-> GAME_OVER
 *            --[waves=max] VICTORY
 *
 *  PAUSED    --[ENTER]---> PLAYING    (resume)
 *            --[ESC]-----> QUIT       (exit game)
 *            --[Q]-------> MAIN_MENU  (reset + menu)
 *
 *  GAME_OVER --[ENTER/ESC] MAIN_MENU  (reset)
 *  VICTORY   --[ENTER/ESC] MAIN_MENU  (reset)
 * ------------------------------------------------------- */
typedef enum {
    STATE_MAIN_MENU = 0,
    STATE_PLANT_SELECTION,  /* ecrã de escolha de plantas antes do jogo */
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER,
    STATE_VICTORY,
    STATE_QUIT        /* sentinel: exits the interrupt loop */
} GameState;

/* Get the current game state */
GameState get_game_state(void);

/* Transition to a new state */
void set_game_state(GameState new_state);

/* --- Game Info --- */
typedef struct {
    int sun_count;
    int score;
    int zombie_spawn_rate;
    int wave;
} GameInfo;

/* Get the current game info */
const GameInfo* get_game_info(void);
void add_sun(int amount);
bool spend_sun(int amount);
void set_game_wave(int wave);

/* Initialize/reset game info defaults */
void init_game_info(void);

#endif /* _STATE_H_ */
