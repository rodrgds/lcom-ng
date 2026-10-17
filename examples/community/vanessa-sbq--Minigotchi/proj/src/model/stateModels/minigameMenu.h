#ifndef _LCOM_MINIGAMEMENU_H_
#define _LCOM_MINIGAMEMENU_H_

#include "../../viewer/guiDrawer.h"

#include "../button.h"
#include "../cursor.h"
#include "../minigotchi.h"

typedef struct{
    Button *tttButton;
    Button *rpsButton;
    Button *quitButton;
    Cursor *cursor;
} MinigameMenu;

// "Constructor"
MinigameMenu *new_minigameMenu(); 

void delete_minigameMenu(MinigameMenu *minigameMenu);

Button* minigameMenu_get_tttButton(MinigameMenu *minigameMenu);
Button* minigameMenu_get_rpsButton(MinigameMenu *minigameMenu);
Button* minigameMenu_get_quitButton(MinigameMenu *minigameMenu);
Cursor* minigameMenu_get_cursor(MinigameMenu *minigameMenu);

#endif /* _MINIGAMEMENU_H */
