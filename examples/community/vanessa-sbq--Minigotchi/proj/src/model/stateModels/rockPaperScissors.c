#include "rockPaperScissors.h"


/**
 * @brief Creates a new RockPaperScissors "object" and returns a pointer to it.
 */
RockPaperScissors *new_rockPaperScissors(){
    // Allocate RockPaperScissors object
    RockPaperScissors *rockPaperScissors = malloc(sizeof(*rockPaperScissors));
    if (rockPaperScissors == NULL) return NULL;

    // Initialize RockPaperScissors
    rockPaperScissors->rockButton = new_button(500, 150, 3);
    rockPaperScissors->paperButton = new_button(510, 350, 4);
    rockPaperScissors->scissorsButton = new_button(500, 550, 5);
    rockPaperScissors->quitButton = new_button(60, 690, 1);
    rockPaperScissors->cursor = new_cursor(400, 600);
    set_buttonClicked(true, rockPaperScissors->cursor);
    rockPaperScissors->rps_state = RPS_PLAYING;

    srand(time(NULL));
    int reward_id = (rand() % NUM_ITEMS) + 1;
    rockPaperScissors->reward = new_item(400, 500, reward_id, 1, 100);

    return rockPaperScissors;
}

/**
 * @brief RockPaperScissors destructor
*/
void delete_rockPaperScissors(RockPaperScissors *rockPaperScissors){ 
    if (rockPaperScissors != NULL) {
        button_delete(rockPaperScissors->rockButton);
        button_delete(rockPaperScissors->paperButton);
        button_delete(rockPaperScissors->scissorsButton);
        button_delete(rockPaperScissors->quitButton);
        delete_cursor(rockPaperScissors->cursor);
        free(rockPaperScissors);
    }
}


// Getters and Setters

Button* rockPaperScissors_get_rockButton(RockPaperScissors *rockPaperScissors){
    return rockPaperScissors->rockButton;
}

Button* rockPaperScissors_get_paperButton(RockPaperScissors *rockPaperScissors){
    return rockPaperScissors->paperButton;
}

Button* rockPaperScissors_get_scissorsButton(RockPaperScissors *rockPaperScissors){
    return rockPaperScissors->scissorsButton;
}

Button* rockPaperScissors_get_quitButton(RockPaperScissors *rockPaperScissors){
    return rockPaperScissors->quitButton;
}

Cursor* rockPaperScissors_get_cursor(RockPaperScissors *rockPaperScissors){
    return rockPaperScissors->cursor;
}

rps_state_t rockPaperScissors_get_rpsState(RockPaperScissors *rockPaperScissors){
    return rockPaperScissors->rps_state;
}

Item* rockPaperScissors_get_reward(RockPaperScissors *rockPaperScissors){
    return rockPaperScissors->reward;
}

collision_type_rps_t rockPaperScissors_get_botChoice(RockPaperScissors *rockPaperScissors){
    return rockPaperScissors->bot_choice;
}

void rockPaperScissors_set_rpsState(RockPaperScissors *rockPaperScissors, rps_state_t rpsState){
    rockPaperScissors->rps_state = rpsState;
}

void rockPaperScissors_set_reward(RockPaperScissors *rockPaperScissors, Item* reward){
    rockPaperScissors->reward = reward;
}

void rockPaperScissors_set_botChoice(RockPaperScissors *rockPaperScissors, collision_type_rps_t bot_choice){
    rockPaperScissors->bot_choice = bot_choice;
}
