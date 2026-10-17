#include "nameMinigotchi.h"

/**
 * @brief Creates a new NameMinigotchi "object" and returns a pointer to it.
 */
NameMinigotchi *new_nameMinigotchi() {
    NameMinigotchi *nameMinigotchi = malloc(sizeof(*nameMinigotchi));
    if (nameMinigotchi == NULL) return NULL;

    nameMinigotchi->continueButton = new_button(401, 400, 0);
    nameMinigotchi->quitButton = new_button(401, 600, 1);
    nameMinigotchi->cursor = new_cursor(400, 600);
    return nameMinigotchi;
}

/**
 * @brief NameMinigotchi destructor
 */
void delete_nameMinigotchi(NameMinigotchi *nameMinigotchi) {
    free(nameMinigotchi);
}


// Getters and Setters

Button* nameMinigotchi_get_continueButton(NameMinigotchi *nameMinigotchi) {
    return nameMinigotchi->continueButton;
}

Button* nameMinigotchi_get_quitButton(NameMinigotchi *nameMinigotchi) {
    return nameMinigotchi->quitButton;
}

Cursor* nameMinigotchi_get_cursor(NameMinigotchi *nameMinigotchi) {
    return nameMinigotchi->cursor;
}


void nameMinigotchi_setName(NameMinigotchi *nameMinigotchi, char* nm) {
    nameMinigotchi->name = nm;
}

char* nameMinigotchi_getName(NameMinigotchi* nameMinigotchi) {
    return nameMinigotchi->name;
}

