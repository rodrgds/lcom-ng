/**
 * @file text.c
 * @brief Implementation of text rendering and management.
 *
 * This file contains the implementation of the functions declared in text.h.
 */
#include "text.h"

/**
 * @struct text
 * @brief Structure representing a text object for rendering.
 *
 * Contains a pointer to a sprite used for drawing characters on the screen.
 */
struct text {
  sprite_t* sp; /**< Sprite used for rendering text characters. */
};

text_t *create_text(uint16_t x, uint16_t y) {
  xpm_map_t alphabet[27] = {A_big, B_big, C_big, D_big, E_big, F_big, G_big, H_big, I_big, J_big, K_big, L_big, M_big, N_big, O_big, P_big, Q_big, R_big, S_big, T_big, U_big, V_big, W_big, X_big, Y_big, Z_big, espaco_big};

  text_t* new_text = malloc(sizeof(text_t));
  if (!new_text) return NULL;

  new_text->sp = create_sprite(alphabet, 27, x, y);
  if (!new_text->sp) {
    free(new_text);
    return NULL;
  }

  return new_text;
}

void destroy_text(text_t* text) {
  if (text) {
    if (text->sp) {
      destroy_sprite(text->sp);
    }
    free(text);
  }
}

int draw_text(text_t* text, const char* str) {
  if (!text || !text->sp || !str) {
    return 1;
  }

  uint16_t char_width = get_sprite_width(text->sp);

  while (*str) {
    char c = toupper((unsigned char)*str);
    int index;

    //Find out what to draw 
    if (c >= 'A' && c <= 'Z') {
      index = c - 'A';
    } else if (c == ' ') {
      index = 26;
    } else {
      return 1;
    }

    set_sprite_curr(text->sp, index);
    position_t *pos = get_sprite_pos(text->sp);
    pos->x += char_width + char_width/2;
    if (draw_sprite(text->sp)) {
      return 1;
    }
    str++;
  }
  return 0;
}

sprite_t* get_text_sprite(text_t *text) {
  return text->sp;
}
