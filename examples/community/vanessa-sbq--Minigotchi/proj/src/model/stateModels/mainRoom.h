#ifndef _LCOM_MAINROOM_H_
#define _LCOM_MAINROOM_H_

#include "../../viewer/guiDrawer.h"

#include "../button.h"
#include "../bar.h"
#include "../cursor.h"
#include "../minigotchi.h"
#include "../hotbar.h"

typedef struct{
    Button *miniGamesButton;
    Button *quitButton;
    Bar* happinessBar;
    Bar* hungerBar;
    Minigotchi* minigotchi;
    Cursor *cursor;
    Hotbar *hotbar;
} MainRoom;

MainRoom *new_mainRoom(); // "Constructor"

void delete_mainRoom(MainRoom *mainRoom);

Minigotchi* mainRoom_get_minigotchi(MainRoom *mainRoom);
Button* mainRoom_get_miniGamesButton(MainRoom *mainRoom);
Button* mainRoom_get_quitButton(MainRoom *mainRoom);
Bar* mainRoom_get_happinessBar(MainRoom *mainRoom);
Bar* mainRoom_get_hungerBar(MainRoom *mainRoom);
Cursor* mainRoom_get_cursor(MainRoom *mainRoom);
Hotbar* mainRoom_get_hotbar(MainRoom *mainRoom);

#endif /* _MAINROOM_H */
