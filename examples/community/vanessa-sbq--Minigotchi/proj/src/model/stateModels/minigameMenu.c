#include "minigameMenu.h"

/**
 * @brief Creates a new MinigameMenu "object" and returns a pointer to it.
 */
MinigameMenu *new_minigameMenu(){

    // Allocate MinigameMenu object
    MinigameMenu *minigameMenu = malloc(sizeof(*minigameMenu));
    if (minigameMenu == NULL) return NULL;

    // Initialize MinigameMenu
    minigameMenu->tttButton = new_button(307, 165, 9);
    minigameMenu->rpsButton = new_button(307, 455, 10);
    minigameMenu->quitButton = new_button(10, 740, 1);
    minigameMenu->cursor = new_cursor(400, 600);

    return minigameMenu;
}

/**
 * @brief MinigameMenu destructor
 */
void delete_minigameMenu(MinigameMenu *minigameMenu){ 
    if (minigameMenu != NULL) {
        button_delete(minigameMenu->tttButton);
        button_delete(minigameMenu->rpsButton);
        button_delete(minigameMenu->quitButton);
        delete_cursor(minigameMenu->cursor);
        free(minigameMenu);
    }
}


// Getters and Setters

Button* minigameMenu_get_tttButton(MinigameMenu *minigameMenu){
    return minigameMenu->tttButton;
}

Button* minigameMenu_get_rpsButton(MinigameMenu *minigameMenu){
    return minigameMenu->rpsButton;
}

Button* minigameMenu_get_quitButton(MinigameMenu *minigameMenu){
    return minigameMenu->quitButton;
}

Cursor* minigameMenu_get_cursor(MinigameMenu *minigameMenu){
    return minigameMenu->cursor;
}
