#include "minigameMenuController.h"

static MinigameMenu *minigameMenu = NULL;
static collision_type_minigameMenu_t currentButtonEvent = NOP_MINIGAMEMENU;

/**
 * @brief Load the minigame selection menu (first time)
 */
void minigameMenuController_load_minigameMenu(){
    if (minigameMenu == NULL){
        minigameMenu = new_minigameMenu();
        minigameMenuViewer_setMinigameMenu(minigameMenu);
    }
}

/**
 * @brief Function that checks sprite collisions with the cursor
 */
bool minigameMenuController_checkCollision(Sprite* sprite, int x, int y) {
    Cursor* cursor = minigameMenu_get_cursor(minigameMenu);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);

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

collision_type_minigameMenu_t minigameMenuController_getButtonEvent() {
    return currentButtonEvent;
}

void minigameMenuController_setButtonEvent(collision_type_minigameMenu_t ct){
    currentButtonEvent = ct;
}

/**
 * @brief Controller function that is called once every frame and updates / checks every action in the minigame selection menu
 */
void minigameMenuController_step(){
    minigameMenuController_load_minigameMenu();
    
    // Check if "Quit" clicked
    Button* quit_button = minigameMenu_get_quitButton(minigameMenu);
    if (minigameMenuController_checkCollision(button_get_sprite(quit_button), button_get_x(quit_button), button_get_y(quit_button))){
        currentButtonEvent = QUIT_MINIGAMEMENU;
    }

    // Check if "Tic tac toe" clicked
    Button* ttt_button = minigameMenu_get_tttButton(minigameMenu);
    if (minigameMenuController_checkCollision(button_get_sprite(ttt_button), button_get_x(ttt_button), button_get_y(ttt_button))){
        currentButtonEvent = MINIGAME1_MINIGAMEMENU;
    }

    // Check if "Rock Paper Scissors" clicked
    Button* rps_button = minigameMenu_get_rpsButton(minigameMenu);
    if (minigameMenuController_checkCollision(button_get_sprite(rps_button), button_get_x(rps_button), button_get_y(rps_button))){
        currentButtonEvent = MINIGAME2_MINIGAMEMENU;
    }
}

/**
 * @brief Calls the MinigameMenu destructor
 */
void minigameMenuController_delete_minigameMenu(){
    delete_minigameMenu(minigameMenu);
}

Cursor* getMinigameMenuCursor() {
    currentButtonEvent = NOP_MINIGAMEMENU;
    return minigameMenu->cursor;
}

void setMinigameMenuCursor(Cursor* cursor) {
    minigameMenu->cursor = cursor;
}
