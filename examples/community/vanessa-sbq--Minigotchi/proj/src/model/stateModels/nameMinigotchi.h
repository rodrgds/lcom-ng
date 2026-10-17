#ifndef _LCOM_NAME_MINIGOTCHI_H_
#define _LCOM_NAME_MINIGOTCHI_H_

#include "../../viewer/guiDrawer.h"

#include "../button.h"
#include "../cursor.h"
#include "../../utilities/vector.h"

typedef struct{
    Button *continueButton;
    Button *quitButton;
    Cursor *cursor;
    char *name;
} NameMinigotchi;

NameMinigotchi *new_nameMinigotchi(); // "Constructor"

void delete_nameMinigotchi(NameMinigotchi *nameMinigotchi); // "Destructor"

// Getters and setters

Button* nameMinigotchi_get_continueButton(NameMinigotchi *nameMinigotchi);
Button* nameMinigotchi_get_quitButton(NameMinigotchi *nameMinigotchi);
Cursor* nameMinigotchi_get_cursor(NameMinigotchi *nameMinigotchi);

void nameMinigotchi_setName(NameMinigotchi *nameMinigotchi, char* nm);
char* nameMinigotchi_getName(NameMinigotchi *nameMinigotchi);

#endif /* _NAME_MINIGOTCHI_H */
