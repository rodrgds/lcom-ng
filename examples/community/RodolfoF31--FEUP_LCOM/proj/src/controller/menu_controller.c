#include "menu_controller.h"

void init_menu(MenuState *menu) {
  static MenuItem predefined_items[] = {
    {"Easy mode", true},
    {"Hard mode", false},
    {"Exit game", false}};
  menu->items = predefined_items;
  menu->item_count = sizeof(predefined_items) / sizeof(predefined_items[0]);
  menu->selected_index = 0;

  int x_start = 350; // Center horizontally 
  int y_start = get_vertical_resolution() / 4;  

  static Arrow arrow;
  for (int i = 0; i < menu->item_count; i++) {
    arrow.allPosition[i].x = x_start - 75; // Arrow X position (left of label)
    arrow.allPosition[i].y = y_start + MENU_LINE_SPACING * (i + 1); // Y position for each item
  }
  arrow.index_prev_position = 0;
  arrow.index_position = 0;

  menu->select_arrow = arrow;

  return;
}

void update_menu(MenuState *menu, int direction) {
  menu->select_arrow.index_prev_position = menu->selected_index; // Save previous index
  menu->selected_index += direction;
  if (menu->selected_index < 0) {
    menu->selected_index = menu->item_count - 1;
  }
  else if (menu->selected_index >= menu->item_count) {
    menu->selected_index = 0;
  }
  menu->select_arrow.index_position = menu->selected_index; // Update current index

}

void draw_menu(MenuState *menu) {

  clearScreen();

  int x_start = 350; // Center horizontally
  int y_start = get_vertical_resolution() / 4;   // Start vertically

  // Draw the title
  draw_sentence("Welcome to LCOM Typeracer", 100, 100, "white"); // Yellow
  y_start += MENU_LINE_SPACING ;



  // Draw each menu item
  for (int i = 0; i < menu->item_count; i++) {
    if (i == menu->selected_index) {
      draw_sentence(">", x_start - 75, y_start,"white");
      draw_sentence(menu->items[i].label, x_start, y_start, "white"); // White for selected
    }
    else {
      draw_sentence(menu->items[i].label , x_start, y_start, "white"); // Green for unselected
    }
    y_start += MENU_LINE_SPACING ;
  }

  draw_sentence("GRUPO 2LEIC18 1", 70, 700, "white"); 
}


GameConfig select_menu_item(MenuState *menu) {
  return handle_menu_selection(menu->items[menu->selected_index].label);
}

GameConfig handle_menu_selection(const char *option){
  if (strcmp(option, "Easy mode") == 0) {
    GameConfig config = { "easy", 30, false, false };
    return config;
  } else if (strcmp(option, "Hard mode") == 0) {
    GameConfig config = { "hard", 45, true, true };
    return config;
  } else if (strcmp(option, "Exit game") == 0) {
      GameConfig config = { "exit", 0, false, false };
      return config;
  } else {
      printf("Invalid option selected.\n");
  }
  GameConfig config = { "invalid", 0, false, false };
  return config;
}

void draw_update(MenuState *menu) {

  
  int x_prev = menu->select_arrow.allPosition[menu->select_arrow.index_prev_position].x;
  int y_prev = menu->select_arrow.allPosition[menu->select_arrow.index_prev_position].y;

  // Get arrow size from XPM
  int arrow_width = get_xpm_width(arrow);
  int arrow_height = get_xpm_height(arrow);

  // Draw black rectangle over previous arrow
  vg_draw_rectangle(x_prev, y_prev - arrow_height, arrow_width, arrow_height, 0x000000);

  int x_arrow = menu->select_arrow.allPosition[menu->select_arrow.index_position].x;
  int y_arrow = menu->select_arrow.allPosition[menu->select_arrow.index_position].y;

  draw_sentence(">", x_arrow, y_arrow,"white"); // Draw new arrow position

}
