#include "mainMenuController.h"

static MainMenu *mainMenu = NULL;
static collision_type_t currentButtonEvent = START;

/**
 * @brief Load the main menu (first time)
 */
void mainMenuController_load_mainMenu(){
    if (mainMenu == NULL){
        mainMenu = new_mainMenu();
        mainMenuViewer_setMainMenu(mainMenu);
    }
}

/**
 * @brief Function that checks button collisions with the cursor
 */
collision_type_t mainMenuController_checkCollision(Button *button) {
    Cursor* cursor = mainMenu_get_cursor(mainMenu);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);
    
    int button_width = sprite_get_width(button_get_sprite(button));
    int button_height = sprite_get_height(button_get_sprite(button));

    int left_upper_corner_x = button_get_x(button);

    int right_upper_corner_x = left_upper_corner_x + button_width;

    int right_upper_corner_y = button_get_y(button);

    int left_lower_corner_y = right_upper_corner_y + button_height;

    if ((cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) && (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) && beingPressed) {
            return currentButtonEvent;
    }

    return NOP;
}

collision_type_t mainMenuController_getButtonEvent() {
    return currentButtonEvent;
}

/**
 * @brief Controller function that is called once every frame and updates / checks every action in the main menu
 */
void mainMenuController_step(){
    mainMenuController_load_mainMenu();

    collision_type_t pressedButton = NOP;

    currentButtonEvent = START;
    pressedButton = mainMenuController_checkCollision(mainMenu_get_startNewButton(mainMenu));

    if (pressedButton == currentButtonEvent) {
        return;
    }

    currentButtonEvent = QUIT;
    pressedButton = mainMenuController_checkCollision(mainMenu_get_quitButton(mainMenu));

    if (pressedButton == currentButtonEvent) {
        return;
    }

    currentButtonEvent = NOP;
}

/**
 * @brief Calls the main menu destructor
 */
void mainMenuController_delete_mainMenu(){
    delete_mainMenu(mainMenu);
}

Cursor* getMainMenuCursor() {
    return mainMenu->cursor;
}

void setMainMenuCursor(Cursor* cursor) {
    mainMenu->cursor = cursor;
}
