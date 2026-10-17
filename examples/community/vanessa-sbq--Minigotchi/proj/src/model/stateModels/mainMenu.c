#include "mainMenu.h"

/**
 * @brief Creates a new MainMenu "object" and returns a pointer to it.
 */
MainMenu *new_mainMenu(){
    // Allocate MainMenu object
    MainMenu *mainMenu = malloc(sizeof(*mainMenu));
    if (mainMenu == NULL) return NULL;

    // Initialize MainMenu
    mainMenu->startNewButton = new_button(401, 400, 0);
    mainMenu->quitButton = new_button(401, 600, 1);
    mainMenu->cursor = new_cursor(400, 600);
    return mainMenu;
}

/**
 * @brief MainMenu destructor
 */
void delete_mainMenu(MainMenu *mainMenu){
    button_delete(mainMenu->startNewButton);
    button_delete(mainMenu->quitButton);
    free(mainMenu);
}


// Getters and setters

Button* mainMenu_get_startNewButton(MainMenu *mainMenu){
    return mainMenu->startNewButton;
}

Button* mainMenu_get_quitButton(MainMenu *mainMenu){
    return mainMenu->quitButton;
}

Cursor* mainMenu_get_cursor(MainMenu *mainMenu){
    return mainMenu->cursor;
}
