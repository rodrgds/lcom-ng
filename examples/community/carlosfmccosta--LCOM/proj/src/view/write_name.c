/**
 * @file write_name.c
 * @brief Implementation of write-name page management.
 *
 * This file contains the implementation of the functions declared in write_name.h.
 */
#include "write_name.h"

/**
 * @struct write_name
 * @brief Structure representing the write-name page.
 *
 * Contains all data and resources needed to render and manage the write-name page,
 * including text objects, title and cue strings, player name, menu options, callbacks,
 * cursor position, and state flags.
 */
struct write_name {
  text_t *text;             /**< Text object for rendering. */
  position_t *pos;          /**< Position for the title text. */
  char *name;               /**< The name typed by the player. */
  char *title;              /**< Page title string. */
  char *cue;                /**< Prompt string ("Write your name:"). */
  bool writing;             /**< Flag indicating if the user is currently writing. */
  sprite_t *option;         /**< Sprite for menu options. */
  char **options;           /**< Array of option strings. */
  void (**callbacks)(void); /**< Array of callback functions for options. */
  unsigned int size;        /**< Number of options. */
  int opt;                  /**< Currently selected option index. */
  position_t *cur_pos;      /**< Position to place the cursor. */
  unsigned int name_len;    /**< Length of the player's name. */
};

write_name_t *create_write_name_page(const char *title, const char *cue, const char **options, void (**callbacks)(void), unsigned int size, unsigned h_res, unsigned v_res) {
  if (!title || !cue || !options || !callbacks || size == 0) {
    return NULL;
  }

  write_name_t *page = malloc(sizeof(write_name_t));
  if (!page)
    return NULL;

  // Load text and sprites
  text_t *text = create_text(0, 0);
  if (!text) {
    free(page);
    return NULL;
  }

  xpm_map_t opt_xpms[2] = {menu_option, menu_option_sel};
  sprite_t *opt = create_sprite(opt_xpms, 2, 0, 0);
  if (!opt) {
    destroy_text(text);
    free(page);
    return NULL;
  }

  sprite_t *sp = get_text_sprite(text);
  unsigned int char_width = get_sprite_width(sp);
  unsigned int title_len = strlen(title);
  unsigned int total_width = title_len * (char_width + char_width / 2);
  int x = (h_res - total_width) / 2;
  int y = v_res / 4;

  position_t *pos = get_sprite_pos(sp);
  pos->x = x;
  pos->y = y;

  // Fill structure
  page->text = text;
  page->pos = pos;
  page->name = calloc(MAX_NAME_CHARS + 1, sizeof(char));
  page->title = strdup(title);
  page->cue = strdup(cue);
  page->option = opt;
  page->options = (char **) options;
  page->size = size;
  page->callbacks = callbacks;
  page->opt = -1;
  page->writing = true;
  page->name_len = 0;

  uint16_t my_y = page->pos->y;

  // Draw typed
  uint16_t temp_y = my_y + (get_sprite_height(sp) + 20) * 2;
  uint16_t cur_pos_x = (get_hres() - get_sprite_width(page->option)) / 2 + 10;
  uint16_t cur_pos_y = temp_y + get_sprite_height(sp) / 2;

  // Calculate position
  position_t *cur_pos = malloc(sizeof(position_t));
  if (!pos) {
    destroy_sprite(opt);
    destroy_text(text);
    free(page);
    return NULL;
  }
  cur_pos->x = cur_pos_x;
  cur_pos->y = cur_pos_y;
  page->cur_pos = cur_pos;

  return page;
}

void destroy_write_name_page(write_name_t *page) {
  if (!page) {
    return;
  }

  if (page->text) {
    destroy_text(page->text);
  }
  if (page->option) {
    destroy_sprite(page->option);
  }
  if (page->name) {
    free(page->name);
  }
  if (page->title) {
    free(page->title);
  }
  if (page->cue) {
    free(page->cue);
  }

  if (page->options) {
    for (unsigned int i = 0; i < page->size; i++) {
      if (page->options[i]) {
        free((void *) page->options[i]);
      }
    }
    free(page->options);
  }

  if (page->callbacks) {
    free(page->callbacks);
  }

  free(page);
}

int draw_write_name_page(write_name_t *page) {
  if (!page) {
    return 1;
  }

  sprite_t *text_sprite = get_text_sprite(page->text);
  position_t *text_pos = get_sprite_pos(text_sprite);

  int16_t base_x = text_pos->x;
  uint16_t base_y = text_pos->y;

  int16_t x = page->pos->x;
  uint16_t y = page->pos->y;

  unsigned int option_width = get_sprite_width(page->option);
  unsigned int char_width = get_sprite_width(text_sprite);

  // Draw title
  text_pos->x = x;
  text_pos->y = y;
  draw_text(page->text, page->title);

  // Draw cue
  text_pos->x = x - 5 * get_sprite_width(text_sprite);
  text_pos->y = y + get_sprite_height(text_sprite) + 20;
  draw_text(page->text, page->cue);

  // Draw typed
  text_pos->y += get_sprite_height(text_sprite) + 20;
  uint16_t temp_y = text_pos->y;
  position_t *opt_pos = get_sprite_pos(page->option);
  opt_pos->x = (get_hres() - get_sprite_width(page->option)) / 2;
  opt_pos->y = temp_y;
  set_sprite_curr(page->option, 0);
  if (draw_sprite(page->option)) {
    return 1;
  }
  text_pos->x = opt_pos->x;
  text_pos->y = temp_y + get_sprite_height(text_sprite) / 2;
  draw_text(page->text, page->name);

  text_pos->x = base_x;
  text_pos->y = base_y;

  // Draw options
  for (unsigned int i = 0; i < page->size; i++) {
    int opt_y = y + 3 * get_sprite_height(text_sprite) + 60 + i * (get_sprite_height(page->option) + 10);

    opt_pos->y = opt_y;

    // Set correct bg
    if (page->opt == -1) {
      set_sprite_curr(page->option, 0);
    }
    else if (page->opt == (int) i) {
      set_sprite_curr(page->option, 1);
    }
    else {
      set_sprite_curr(page->option, 0);
    }
    if (draw_sprite(page->option)) {
      return 1;
    }

    unsigned int text_len = strlen(page->options[i]);
    unsigned int text_width = text_len * (char_width * 2);

    int text_x = opt_pos->x + (option_width - text_width) / 2;
    int text_y = opt_y + get_sprite_height(text_sprite) / 2;

    int temp_x = text_pos->x, temp_y = text_pos->y;
    text_pos->x = text_x;
    text_pos->y = text_y;

    if (draw_text(page->text, page->options[i])) {
      return 1;
    }

    // Reset pos
    text_pos->x = temp_x;
    text_pos->y = temp_y;
  }

  return 0;
}

char *get_player_name(write_name_t *page) {
  return page->name;
}

bool is_writing(write_name_t *page) {
  return page->writing;
}

void set_page_writing(write_name_t *page, bool writing) {
  page->writing = writing;
}

unsigned int get_page_opt(write_name_t *page) {
  return page->opt;
}

void set_page_opt(write_name_t *page, int opt) {
  page->opt = opt;
}

unsigned int get_page_size(write_name_t *page) {
  return page->size;
}

void (*get_page_callback(write_name_t *page, unsigned int opt))(void) {
  if (!page || !page->callbacks || opt >= page->size) {
    return NULL;
  }
  return page->callbacks[opt];
}

sprite_t *get_page_option(write_name_t *page) {
  return page->option;
}

uint16_t get_page_title_y(const write_name_t *page) {
  return page->pos->y;
}

unsigned int get_name_lenght(const write_name_t *page) {
  return page->name_len;
}

void set_name_lenght(write_name_t *page, unsigned int len) {
  page->name_len = len;
}

position_t *get_page_cur_pos(write_name_t *page) {
  return page->cur_pos;
}

text_t *get_page_text(write_name_t *page) {
  return page->text;
}
