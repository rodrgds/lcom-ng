#include "../include/menu_model.h"

menu_t* 
menu_new(uint16_t width, uint16_t height)
{
  menu_t* new_menu = (menu_t*) malloc(sizeof(menu_t)); 
  if(new_menu == NULL){
    perror("Something Went Wrong while trying to generate game menu."); 
    return NULL; 
  }

  new_menu->width = width; 
  new_menu->height = height;
  new_menu->n_entries = 2;
  new_menu->current_entry = 1;
  new_menu->models[0] = sprite_new((const char* const*)snake_io_xpm, 150,0);
  new_menu->models[1] = sprite_new((const char* const*)start_xpm, 100, 400); 
  new_menu->models[2] = sprite_new((const char* const*)exitbutton_xpm, 500, 400);
  new_menu->mouse_cursor = cursor_new();
  new_menu->mouse_cursor_dbuffer = *(new_menu->mouse_cursor);
  new_menu->mouse_cursor_dbuffer.cursorsprite = sprite_new((const char* const*) cursor_xpm_delete, new_menu->mouse_cursor_dbuffer.x, new_menu->mouse_cursor_dbuffer.y);
  
  new_menu->selected_models[1] = sprite_new((const char* const*)start_selected_xpm, 100, 400);
  new_menu->selected_models[2] = sprite_new((const char* const*)exit_button_selected_xpm, 500, 400);

  return new_menu;
}

void 
menu_delete(menu_t** menu)
{
  sprite_delete((*menu)->models[0]);
  sprite_delete((*menu)->models[1]);
  sprite_delete((*menu)->models[2]);
  sprite_delete((*menu)->selected_models[1]);
  sprite_delete((*menu)->selected_models[2]);
  free(*menu);
  *menu = NULL;
}

uint16_t 
menu_get_width(menu_t* menu)
{
  return menu->width;
}

uint16_t 
menu_get_height(menu_t* menu)
{
  return menu->height;
}

void
menu_next_entry(menu_t* menu)
{
  menu->current_entry++;
  if (menu->current_entry > menu->n_entries)
    menu->current_entry = 1;
}

void
menu_prev_entry(menu_t* menu)
{
  menu->current_entry--;
  if (menu->current_entry < 1)
    menu->current_entry = menu->n_entries;
}

bool
menu_is_selected(menu_t* menu, int i)
{
  return menu->current_entry == i;
}

bool 
menu_is_selected_exit(menu_t* menu)
{
  return menu_is_selected(menu, 2);
}

bool 
menu_is_selected_1p(menu_t* menu)
{
  return menu_is_selected(menu, 1);
}
