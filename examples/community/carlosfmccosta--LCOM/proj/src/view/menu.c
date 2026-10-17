/**
 * @file menu.c
 * @brief Implementation of menu management.
 *
 * This file contains the implementation of the functions declared in menu.h.
 */
#include "menu.h"

/**
 * @struct menu
 * @brief Represents a menu with a title, options, and corresponding actions.
 *
 * Contains pointers to the text object, option/background sprites, title and option strings,
 * callback functions, layout properties, and the current selection state.
 */
struct menu {
  text_t* text;                 /**< Text sprite to draw text. */
  sprite_t* option;             /**< Background for each option. */
  sprite_t* background;         /**< Background sprite for the menu. */
  const char* title;            /**< Title of the menu. */
  const char** options;         /**< Array of strings representing the options. */
  void (**callbacks)(void);     /**< Array of function pointers for each option. */
  uint16_t opt_vertical_offset; /**< Vertical offset for option placement. */
  uint16_t x_pos;               /**< Horizontal position for all options. */
  position_t* pos_title;        /**< Position of the title. */
  unsigned int opt;             /**< Currently selected option. */
  unsigned int size;            /**< Total options in the menu. */
};

menu_t* create_menu(const char *title_str, const char **options, void (**func)(void), unsigned int size, uint16_t h_res, uint16_t v_res) {
  if (!title_str || !options || !func || size == 0) {
    return NULL;
  }

  menu_t* menu = calloc(1, sizeof(menu_t));
  if (!menu) {
    return NULL;
  }

  xpm_map_t opt_xpms[2] = {menu_option, menu_option_sel};
  sprite_t* opt = create_sprite(opt_xpms, 2, 0, 0);
  if (!opt) {
    free(menu);
    return NULL;
  }

  text_t* text = create_text(0, 0);
  if (!text) {
    destroy_sprite(opt);
    free(menu);
    return NULL;
  }

  sprite_t* sp = get_text_sprite(text);
  unsigned int char_width = get_sprite_width(sp);

  // Calculate title width in pixels
  unsigned int title_len = 0;
  for (const char* p = title_str; *p != '\0'; p++) {
    title_len++;
  }

  unsigned int total_width = title_len * (char_width + char_width / 2 );
  int x = (h_res - total_width) / 2;
  int y = (v_res / 2) - get_sprite_height(sp) * size;

  // Set calculated position
  position_t* pos = get_sprite_pos(sp);
  pos->x = x;
  pos->y = y;

  // Assign menu fields
  menu->text = text;
  menu->pos_title = pos;
  menu->option = opt;
  menu->background = NULL;
  menu->size = size;
  menu->options = calloc(size, sizeof(*menu->options));
  menu->callbacks = malloc(size * sizeof(*menu->callbacks));
  menu->opt = 0;
  menu->title = strdup(title_str);
  if (!menu->options || !menu->callbacks || !menu->title) {
    destroy_menu(menu);
    return NULL;
  }
  for (unsigned int i = 0; i < size; i++) {
    if (!options[i] || !(menu->options[i] = strdup(options[i]))) {
      destroy_menu(menu);
      return NULL;
    }
  }
  memcpy(menu->callbacks, func, size * sizeof(*menu->callbacks));
  menu->opt_vertical_offset = get_sprite_height(opt) + 10;
  menu->x_pos = (h_res - get_sprite_width(opt)) / 2; 

  return menu;
}

void change_menu_opt(menu_t *menu, unsigned int opt) {
  menu->opt = opt;
}

void destroy_menu(menu_t *menu) {
  if (!menu){
    return;
  }

  if (menu->text){
    destroy_text(menu->text);
  }

  if (menu->option){
    destroy_sprite(menu->option);
  }

  if (menu->background){
    destroy_sprite(menu->background);
  }

  if (menu->title) {
    free((void*)menu->title);
  }

  if (menu->options) {
    for (unsigned int i = 0; i < menu->size; i++) {
      if (menu->options[i]) {
        free((void*)menu->options[i]);
      }
    }
    free(menu->options);
  }

  if (menu->callbacks) {
    free(menu->callbacks);
  }

  free(menu);
}

int draw_menu(menu_t* menu) {
  if (!menu){
    return 1;
  }

  //Draw bg
  if (menu->background) {
    if (draw_sprite(menu->background)) {
      return 1;
    }
  }

  sprite_t* text_sprite = get_text_sprite(menu->text);

  // Draw title
  position_t* text_pos = get_sprite_pos(text_sprite);

  int title_x = menu->pos_title->x;
  int title_y = menu->pos_title->y;

  text_pos->x = title_x;
  text_pos->y = title_y;

  draw_text(menu->text, menu->title);

  // Draw each option
  for (unsigned int i = 0; i < menu->size; i++) {
    int opt_y = (menu->pos_title->y) + (i + 1) * menu->opt_vertical_offset;

    // Draw options
    position_t* opt_pos = get_sprite_pos(menu->option);
    opt_pos->x = menu->x_pos;
    opt_pos->y = opt_y;

    if (menu->opt == i) {
      set_sprite_curr(menu->option, 1);
    } else {
      set_sprite_curr(menu->option, 0);
    }
    if (draw_sprite(menu->option)) {
      return 1;
    }

    // Draw the text
    unsigned int option_width = get_sprite_width(menu->option);

    unsigned int char_width = get_sprite_width(text_sprite);
    unsigned int text_len = strlen(menu->options[i]);
    unsigned int text_width = text_len * (char_width * 2);

    int text_x = menu->x_pos + (option_width - text_width) / 2;
    int text_y = opt_y + get_sprite_height(text_sprite) /2;

    int temp_x = text_pos->x;
    int temp_y = text_pos->y;

    text_pos->x = text_x;
    text_pos->y = text_y;

    if (draw_text(menu->text, menu->options[i])) {
      return 1;
    }

    //restaurar posiçao original do text
    text_pos->x = temp_x;
    text_pos->y = temp_y;
  }

  text_pos->x = title_x;
  text_pos->y = title_y;
  return 0;
}

unsigned int get_menu_opt(menu_t* menu) {
  return menu->opt;
}

void set_menu_opt(menu_t* menu, unsigned int opt) {
  menu->opt = opt;
}

unsigned int get_menu_size(menu_t* menu) {
  return menu->size;
}

void (*get_menu_callback(menu_t *menu, unsigned int opt))(void) {
  if (!menu || !menu->callbacks || opt >= menu->size) {
    return NULL;
  }
  return menu->callbacks[opt];
}

sprite_t* get_menu_option(menu_t *menu) {
  return menu->option;
}

uint16_t get_menu_x_pos(menu_t *menu) {
  return menu->x_pos;
}

uint16_t get_menu_title_y(const menu_t* menu) {
   return menu->pos_title->y;
}

uint16_t get_menu_opt_vertical_offset(menu_t *menu) {
  return menu->opt_vertical_offset;
}
