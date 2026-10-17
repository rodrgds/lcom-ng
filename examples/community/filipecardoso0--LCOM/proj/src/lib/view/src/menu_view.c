#include "../include/menu_view.h"

int 
menu_draw(menu_t* menu)
{

  if (menu->mouse_cursor->x != menu->mouse_cursor_dbuffer.x ||
      menu->mouse_cursor->y != menu->mouse_cursor_dbuffer.y
  )
    sprite_draw(menu->mouse_cursor_dbuffer.cursorsprite);
  
  sprite_draw(menu->models[0]);

  for (int i = 1; i <= menu->n_entries; i++)
    if (menu_is_selected(menu, i))
      sprite_draw(menu->selected_models[i]);
    else
      sprite_draw(menu->models[i]);

  sprite_draw(menu->mouse_cursor->cursorsprite); 

  menu->mouse_cursor_dbuffer.x = menu->mouse_cursor->x;
  menu->mouse_cursor_dbuffer.y = menu->mouse_cursor->y;
  menu->mouse_cursor_dbuffer.cursorsprite->x = menu->mouse_cursor->x;
  menu->mouse_cursor_dbuffer.cursorsprite->y = menu->mouse_cursor->y;

  return 0; 
}
