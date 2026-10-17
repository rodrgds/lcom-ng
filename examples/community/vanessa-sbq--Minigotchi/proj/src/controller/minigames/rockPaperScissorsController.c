#include "rockPaperScissorsController.h"

static RockPaperScissors *rockPaperScissors = NULL;
static collision_type_rps_t currentButtonEvent = NOP_RPS;
static bool canClick = true;

/**
 * @brief Flag to fix button overlapping (When a button before is clicked)
 */
void rps_set_canClick(){
    canClick = true;
}

/**
 * @brief Load the Rock Paper Scissors minigame (first time)
 */
void rockPaperScissorsController_load_rockPaperScissors(){
    if (rockPaperScissors == NULL){
        rockPaperScissors = new_rockPaperScissors();
        rockPaperScissorsViewer_setRockPaperScissors(rockPaperScissors);
    }
}

/**
 * @brief Function that checks sprite collisions with the cursor
 */
bool rockPaperScissorsController_checkCollision(Sprite* sprite, int x, int y) {
    Cursor* cursor = rockPaperScissors_get_cursor(rockPaperScissors);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);

    if (canClick && beingPressed) return false;
    else canClick = false;

    int sprite_width = sprite_get_width(sprite);
    int sprite_height = sprite_get_height(sprite);

    int left_upper_corner_x = x;
    int right_upper_corner_x = left_upper_corner_x + sprite_width;
    int right_upper_corner_y = y;
    int left_lower_corner_y = right_upper_corner_y + sprite_height;

    if ((cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) && (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) && beingPressed) {
            return true;
    }

    return false;
}

collision_type_rps_t rockPaperScissorsController_getButtonEvent() {
    return currentButtonEvent;
}

void rockPaperScissorsController_setButtonEvent(collision_type_rps_t ct){
    currentButtonEvent = ct;
}

/**
 * @brief Controller function that is called once every frame and updates / checks every action in the Rock Paper Scissors minigame
 */
void rockPaperScissorsController_step(){
    rockPaperScissorsController_load_rockPaperScissors();

    // Check if "Rock" clicked
    Button* rock_button = rockPaperScissors_get_rockButton(rockPaperScissors);
    if (button_get_activated(rock_button)){
        if (rockPaperScissorsController_checkCollision(button_get_sprite(rock_button), button_get_x(rock_button), button_get_y(rock_button))){
            currentButtonEvent = RPS_ROCK;
            rockPaperScissorsController_process_choice();
        }
    }

    // Check if "Paper" clicked
    Button* paper_button = rockPaperScissors_get_paperButton(rockPaperScissors);
    if (button_get_activated(paper_button)){
        if (rockPaperScissorsController_checkCollision(button_get_sprite(paper_button), button_get_x(paper_button), button_get_y(paper_button))){
            currentButtonEvent = RPS_PAPER;
            rockPaperScissorsController_process_choice();
        }
    }

    // Check if "Scissors" clicked
    Button* scissors_button = rockPaperScissors_get_scissorsButton(rockPaperScissors);
    if (button_get_activated(scissors_button)){
        if (rockPaperScissorsController_checkCollision(button_get_sprite(scissors_button), button_get_x(scissors_button), button_get_y(scissors_button))){
            currentButtonEvent = RPS_SCISSORS;
            rockPaperScissorsController_process_choice();
        }
    }

    // Check if "Quit" clicked
    Button* quit_button = rockPaperScissors_get_quitButton(rockPaperScissors);
    if (rockPaperScissorsController_checkCollision(button_get_sprite(quit_button), button_get_x(quit_button), button_get_y(quit_button))){
        currentButtonEvent = QUIT_RPS;
        rockPaperScissorsController_resetGame();
    }

    if (!rockPaperScissors_get_rpsState(rockPaperScissors) == RPS_PLAYING){
        button_set_activated(rockPaperScissors_get_rockButton(rockPaperScissors), false);
        button_set_activated(rockPaperScissors_get_paperButton(rockPaperScissors), false);
        button_set_activated(rockPaperScissors_get_scissorsButton(rockPaperScissors), false);
    }
}

/**
 * @brief Processes a player choice by letting the bot play
 * @details The bot chooses an option randomly and in case the player wins, he gets rewarded with an item
 */
void rockPaperScissorsController_process_choice(){
    srand(time(NULL));
    int bot_choice = rand() % 3; // "AI" that chooses his option
    rockPaperScissors_set_botChoice(rockPaperScissors, bot_choice);

    if (bot_choice == RPS_ROCK){
        if (currentButtonEvent == RPS_PAPER) rockPaperScissors_set_rpsState(rockPaperScissors, RPS_WIN);
        else if (currentButtonEvent == RPS_ROCK) rockPaperScissors_set_rpsState(rockPaperScissors, RPS_TIE);
        else  rockPaperScissors_set_rpsState(rockPaperScissors, RPS_LOSE);
    }
    else if (bot_choice == RPS_PAPER){
        if (currentButtonEvent == RPS_SCISSORS) rockPaperScissors_set_rpsState(rockPaperScissors, RPS_WIN);  
        else if (currentButtonEvent == RPS_PAPER) rockPaperScissors_set_rpsState(rockPaperScissors, RPS_TIE);
        else  rockPaperScissors_set_rpsState(rockPaperScissors, RPS_LOSE);
    }
    else if (bot_choice == RPS_SCISSORS){
        if (currentButtonEvent == RPS_ROCK) rockPaperScissors_set_rpsState(rockPaperScissors, RPS_WIN);
        else if (currentButtonEvent == RPS_SCISSORS) rockPaperScissors_set_rpsState(rockPaperScissors, RPS_TIE);
        else rockPaperScissors_set_rpsState(rockPaperScissors, RPS_LOSE);
    }
    else {
         rockPaperScissors_set_rpsState(rockPaperScissors, RPS_TIE);
    }

    if (rockPaperScissors_get_rpsState(rockPaperScissors) == RPS_WIN){
        Item* reward = rockPaperScissors_get_reward(rockPaperScissors);
        database_add_food_to_array(getDatabase(), item_get_id(reward));
    }
}

/**
 * @brief Function that randomizes the reward (after each game)
 */
void rockPaperScissorsController_randomize_reward(){
    srand(time(NULL));
    int reward_id = (rand() % NUM_ITEMS) + 1;
    Item* next_reward = new_item(700, 500, reward_id, 1, 100);
    rockPaperScissors_set_reward(rockPaperScissors, next_reward);
}

/**
 * @brief Function that resets the game
 */
void rockPaperScissorsController_resetGame(){
    rockPaperScissors_set_rpsState(rockPaperScissors, RPS_PLAYING);
    button_set_activated(rockPaperScissors_get_rockButton(rockPaperScissors), true);
    button_set_activated(rockPaperScissors_get_paperButton(rockPaperScissors), true);
    button_set_activated(rockPaperScissors_get_scissorsButton(rockPaperScissors), true);
    rockPaperScissorsController_randomize_reward(); // Choose a different reward next time
}

/**
 * @brief Calls the Rock Paper Scissors destructor
 */
void rockPaperScissorsController_delete_rockPaperScissors(){
    delete_rockPaperScissors(rockPaperScissors);
}

Cursor* getRockPaperScissorsCursor() {
    currentButtonEvent = NOP_RPS;
    return rockPaperScissors->cursor;
}

void setRockPaperScissorsCursor(Cursor* cursor) {
    rockPaperScissors->cursor = cursor;
}
