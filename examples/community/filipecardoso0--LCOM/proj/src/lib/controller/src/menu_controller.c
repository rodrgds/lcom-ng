#include "../include/menu_controller.h"

int 
menu_step(menu_t* menu, action_t action) 
{

  switch(action) {
    case EXIT: set_app_state_null(); return 1;
    case SELECT: 
      if (menu_is_selected_exit(menu)) { 
        set_app_state_null();
        return 1;
      }
      else if (menu_is_selected_1p(menu)) { 
        set_app_state_game();
        return 1;
      }
      break;
    case LEFT:
      menu_prev_entry(menu);
      break;
    case RIGHT:
      menu_next_entry(menu);
      break;
    default: break;
  }

  struct packet* pp = state_get_packet();
  if (pp == NULL) return 0; 

  if (pp->delta_x > 0) {
    if (menu->mouse_cursor->x + pp->delta_x > (int)get_hres() - menu->mouse_cursor->cursorsprite->width)
      menu->mouse_cursor->x = (int)get_hres() - menu->mouse_cursor->cursorsprite->width;
    else
      menu->mouse_cursor->x += pp->delta_x;
  }
  else if (pp->delta_x < 0) {
    if (menu->mouse_cursor->x + pp->delta_x < 0)
      menu->mouse_cursor->x = 0;
    else
      menu->mouse_cursor->x += pp->delta_x;
  }
  if (pp->delta_y < 0) {
    if (menu->mouse_cursor->y + menu->mouse_cursor->cursorsprite->height - pp->delta_y > (int)get_vres())
      menu->mouse_cursor->y = (int)get_vres() - menu->mouse_cursor->cursorsprite->height;
    else
      menu->mouse_cursor->y -= pp->delta_y;
  }
  else if (pp->delta_y > 0) {
    if (menu->mouse_cursor->y - pp->delta_y < 0)
      menu->mouse_cursor->y = 0;
    else
      menu->mouse_cursor->y -= pp->delta_y;
  }
  
  menu->mouse_cursor->cursorsprite->x = menu->mouse_cursor->x; 
  menu->mouse_cursor->cursorsprite->y = menu->mouse_cursor->y; 

  state_set_packet(NULL); // 212x137

  if (pp->lb &&
      menu->mouse_cursor->cursorsprite->x >= 100 &&  
      menu->mouse_cursor->cursorsprite->x <= 321 &&
      menu->mouse_cursor->cursorsprite->y >= 400 &&
      menu->mouse_cursor->cursorsprite->y <= 537
      ) { 
        state_set_packet(NULL);
        set_app_state_game();
        return 1;
      }

  if (pp->lb &&
      menu->mouse_cursor->cursorsprite->x >= 500 &&  
      menu->mouse_cursor->cursorsprite->x <= 712 &&
      menu->mouse_cursor->cursorsprite->y >= 400 &&
      menu->mouse_cursor->cursorsprite->y <= 537
      ) { 
        menu_delete(&menu);
        state_set_packet(NULL);
        set_app_state_null();
        return 1;
      }

  
  return 0;
}
