#ifndef _LCOM_MAINMENU_H_
#define _LCOM_MAINMENU_H_

#include "../../viewer/guiDrawer.h"

#include "../button.h"
#include "../cursor.h"

typedef struct{
    Button *startNewButton;
    Button *quitButton;
    Cursor *cursor;
} MainMenu;

// "Constructor"
MainMenu *new_mainMenu(); 

// "Destructor"
void delete_mainMenu(MainMenu *mainMenu); 

// Getters and setters
Button* mainMenu_get_startNewButton(MainMenu *mainMenu);
Button* mainMenu_get_quitButton(MainMenu *mainMenu);
Cursor* mainMenu_get_cursor(MainMenu *mainMenu);

#endif /* _MAINMENU_H */
