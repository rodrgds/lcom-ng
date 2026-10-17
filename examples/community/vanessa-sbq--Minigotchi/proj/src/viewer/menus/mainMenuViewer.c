#include "mainMenuViewer.h"

static MainMenu *mainMenu;

void mainMenuViewer_setMainMenu(MainMenu* mm){
    mainMenu = mm;
}


/**
 * @brief Calls draw function of each element the MainMenu contains
 */
void mainMenuViewer_draw(){
    vg_clear_screen();
    wrapper_draw_background(0);

    draw_button(mainMenu_get_startNewButton(mainMenu));
    draw_button(mainMenu_get_quitButton(mainMenu));
    
    draw_cursor(mainMenu->cursor);
    vg_page_flip();
}

