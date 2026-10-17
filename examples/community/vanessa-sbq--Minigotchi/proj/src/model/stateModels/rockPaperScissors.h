#ifndef _LCOM_ROCKPAPERSCISSORS_H_
#define _LCOM_ROCKPAPERSCISSORS_H_

#include "../../viewer/guiDrawer.h"

#include "../button.h"
#include "../cursor.h"
#include "../item.h"

typedef enum {RPS_ROCK, RPS_PAPER, RPS_SCISSORS, NOP_RPS, QUIT_RPS} collision_type_rps_t;
typedef enum {RPS_PLAYING, RPS_WIN, RPS_TIE, RPS_LOSE} rps_state_t;

typedef struct{
    Button *rockButton;
    Button *paperButton;
    Button *scissorsButton;
    rps_state_t rps_state;
    collision_type_rps_t bot_choice;
    Item* reward;

    Button *quitButton;
    Cursor *cursor;
} RockPaperScissors;

// "Constructor"
RockPaperScissors *new_rockPaperScissors();

void delete_rockPaperScissors(RockPaperScissors *rockPaperScissors);

// Getters
Button* rockPaperScissors_get_rockButton(RockPaperScissors *rockPaperScissors);
Button* rockPaperScissors_get_paperButton(RockPaperScissors *rockPaperScissors);
Button* rockPaperScissors_get_scissorsButton(RockPaperScissors *rockPaperScissors);
Button* rockPaperScissors_get_quitButton(RockPaperScissors *rockPaperScissors);
Cursor* rockPaperScissors_get_cursor(RockPaperScissors *rockPaperScissors);
rps_state_t rockPaperScissors_get_rpsState(RockPaperScissors *rockPaperScissors);
Item* rockPaperScissors_get_reward(RockPaperScissors *rockPaperScissors);
collision_type_rps_t rockPaperScissors_get_botChoice(RockPaperScissors *rockPaperScissors);

// Setters
void rockPaperScissors_set_rpsState(RockPaperScissors *rockPaperScissors, rps_state_t rpsState);
void rockPaperScissors_set_reward(RockPaperScissors *rockPaperScissors, Item* reward);
void rockPaperScissors_set_botChoice(RockPaperScissors *rockPaperScissors, collision_type_rps_t bot_choice);

#endif /* _ROCKPAPERSCISSORS_H */
