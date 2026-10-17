/**
 * @file leaderboard_page.c
 * @brief Implementation of leaderboard page management.
 *
 * This file contains the implementation of the functions declared in leaderboard_page.h.
 */
#include "leaderboard_page.h"

extern leaderboard_t entries[MAX_TOP+1];
extern int num_players;

/**
 * @struct lb_page
 * @brief Structure representing the leaderboard page.
 *
 * Holds the sprites and data needed to render the leaderboard page, including
 * the menu option sprite, text for titles and names, digit sprites for scores,
 * the position of the title, and a callback function for the menu option.
 */
struct lb_page {
  sprite_t *option;        /**< Sprite for the menu option */
  text_t *text;            /**< Text for rendering leaderboard titles and names */
  sprite_t *digits;        /**< Sprites for rendering digit characters (scores) */
  position_t *title_pos;   /**< Position for the leaderboard title */
  void (*func)(void);      /**< Callback function for the menu option */
};

lb_page_t *create_lb_page(unsigned int h_res, unsigned int v_res, void (*func)(void)) {
  lb_page_t *lb_page = malloc(sizeof(lb_page_t));
  if (!lb_page) {
    return NULL;
  }

  // Create text object for title
  lb_page->text = create_text(v_res / 8, 0);
  if (!lb_page->text) {
    free(lb_page);
    return NULL;
  }

  sprite_t *txt = get_text_sprite(lb_page->text);

  // Center title horizontally
  unsigned int char_width = get_sprite_width(txt);
  unsigned int char_height = get_sprite_height(txt);

  lb_page->title_pos = malloc(sizeof(position_t));
  if (!lb_page->title_pos) {
    destroy_text(lb_page->text);
    free(lb_page);
    return NULL;
  }
  lb_page->title_pos->x = (h_res - (char_width * 22)) / 2;
  lb_page->title_pos->y = v_res / 8;

  // Create sprite for option
  xpm_map_t opt_xpms[1] = {menu_option_sel};
  lb_page->option = create_sprite(opt_xpms, 1, 0, v_res / 8 + 10 * (char_height + 15));
  if (!lb_page->option) {
    free(lb_page->title_pos);
    destroy_text(lb_page->text);
    free(lb_page);
    return NULL;
  }

  // Center option horizontally
  unsigned int option_width = get_sprite_width(lb_page->option);
  position_t *option_pos = get_sprite_pos(lb_page->option);
  option_pos->x = (h_res - option_width) / 2;

  xpm_map_t digits_map[10] = {zero, um, dois, tres, quatro, cinco, seis, sete, oito, nove};
  lb_page->digits = create_sprite(digits_map, 10, 0, 0);
  if (!lb_page->digits) {
    free(lb_page->title_pos);
    destroy_sprite(lb_page->option);
    destroy_text(lb_page->text);
    free(lb_page);
    return NULL;
  }

  lb_page->func = func;

  // Load entries
  if (get_info_lb("/home/lcom/labs/proj/src/assets/leaderboard.txt", entries)) {
    destroy_lb_page(lb_page);
    return NULL;
  }

  return lb_page;
}

void destroy_lb_page(lb_page_t *lb_page) {
  if (!lb_page) {
    return;
  }
  if (lb_page->option) {
    destroy_sprite(lb_page->option);
  }
  if (lb_page->text) {
    destroy_text(lb_page->text);
  }
  if (lb_page->digits) {
    destroy_sprite(lb_page->digits);
  }
  if (lb_page->title_pos) {
    free(lb_page->title_pos);
  }

  free(lb_page);
}

int draw_leaderboard(lb_page_t *lb_page) {
  if (!lb_page) {
    return 1;
  }

  sprite_t *text_sprite = get_text_sprite(lb_page->text);
  position_t *text_pos = get_sprite_pos(text_sprite);

  int title_x = lb_page->title_pos->x;
  int title_y = lb_page->title_pos->y;

  text_pos->x = title_x;
  text_pos->y = title_y;

  // Draw title
  if (draw_text(lb_page->text, "LEADERBOARD")) {
    return 1;
  }

  unsigned int char_height = get_sprite_height(text_sprite);
  unsigned int option_width = get_sprite_width(lb_page->option);

  position_t *opt_pos = get_sprite_pos(lb_page->option);

  if (draw_sprite(lb_page->option)) {
    return 1;
  }

  unsigned int char_width = get_sprite_width(text_sprite);
  int text_x = opt_pos->x + (option_width / 2 - 2 * (char_width * 2));
  int text_y = opt_pos->y + get_sprite_height(text_sprite) / 2;

  int temp_x = text_pos->x;
  int temp_y = text_pos->y;

  text_pos->x = text_x;
  text_pos->y = text_y;

  if (draw_text(lb_page->text, "BACK")) {
    return 1;
  }

  position_t *digit_pos = get_sprite_pos(lb_page->digits);

  for (int i = 0; i < num_players; i++) {
    int entry_y = title_y + (i + 1) * (char_height + 10);

    // creating the entries string
    char buffer[64];
    strcpy(buffer, entries[i].name);
    char score_buf[16];
    sprintf(score_buf, "%d", entries[i].points);

    // Draw the text
    int text_x = 450;
    int text_y = entry_y + get_sprite_height(text_sprite) / 2;

    text_pos->x = text_x;
    text_pos->y = text_y;

    if (draw_text(lb_page->text, buffer)) {
      return 1;
    }

    for (int j = 0; score_buf[j] != '\0'; j++) {
      int digit = score_buf[j] - '0';
      set_sprite_curr(lb_page->digits, digit);

      digit_pos->x = text_x + 200 + j * get_sprite_width(lb_page->digits);
      digit_pos->y = entry_y;

      if (draw_sprite(lb_page->digits)) {
        return 1;
      }
    }

    // restaurar posiçao original do text
    text_pos->x = temp_x;
    text_pos->y = temp_y;
  }

  text_pos->x = title_x;
  text_pos->y = title_y;

  return 0;
}

void (*get_lb_func(lb_page_t *lb_page))(void) {
  return lb_page->func;
}

sprite_t *get_lb_option(lb_page_t *lb_page) {
  return lb_page->option;
}
