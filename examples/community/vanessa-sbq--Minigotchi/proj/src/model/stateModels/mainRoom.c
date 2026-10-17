#include "mainRoom.h"
#include "../../player_data/database.h"

/**
 * @brief Creates a new MainRoom "object" and returns a pointer to it.
 */
MainRoom *new_mainRoom(){
    // Allocate MainRoom object
    MainRoom *mainRoom = malloc(sizeof(*mainRoom));
    if (mainRoom == NULL) return NULL;

    // Initialize MainRoom
    mainRoom->minigotchi = new_minigotchi(467, 370, 100, 100, "MINIGOTCHI");
    mainRoom->miniGamesButton = new_button(10, 130, 2);
    mainRoom->quitButton = new_button(70, 130, 11);
    mainRoom->happinessBar = new_bar(10, 10, 1000, 0);
    mainRoom->hungerBar = new_bar(10, 60, 1000, 1);
    mainRoom->cursor = new_cursor(400, 600); 
    mainRoom->hotbar = new_hotbar(213, 750, 0);
    return mainRoom;
}

/**
 * @brief MainRoom destructor
 */
void delete_mainRoom(MainRoom *mainRoom){
    kill_minigotchi(mainRoom->minigotchi);
    button_delete(mainRoom->miniGamesButton);
    button_delete(mainRoom->quitButton);
    free(mainRoom);
}

// Getters and Setters

Minigotchi* mainRoom_get_minigotchi(MainRoom *mainRoom) {
    return mainRoom->minigotchi;
}

Button* mainRoom_get_miniGamesButton(MainRoom *mainRoom){
    return mainRoom->miniGamesButton;
}

Button* mainRoom_get_quitButton(MainRoom *mainRoom){
    return mainRoom->quitButton;
}

Bar* mainRoom_get_happinessBar(MainRoom *mainRoom){
    return mainRoom->happinessBar;
}

Bar* mainRoom_get_hungerBar(MainRoom *mainRoom){
    return mainRoom->hungerBar;
}

Cursor* mainRoom_get_cursor(MainRoom *mainRoom){
    return mainRoom->cursor;
}

Hotbar* mainRoom_get_hotbar(MainRoom *mainRoom) {
    return mainRoom->hotbar;
}
