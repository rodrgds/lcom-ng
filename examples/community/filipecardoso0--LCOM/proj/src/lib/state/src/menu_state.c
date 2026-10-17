#include "../include/menu_state.h"

menu_t* menu = NULL;

void 
menu_state_step(action_t action)
{

  if (action == EXIT) {
    set_app_state_null();
    menu_delete(&menu);
    return;
  }

  if (menu == NULL) {
    paint_screen(BLACK);
    menu = menu_new(500, 500);
  }
  
  if (menu_step(menu, action)) {
    menu_delete(&menu);
    return;
  }
  state_set_action(NOTHING);
  menu_draw(menu); 
}
