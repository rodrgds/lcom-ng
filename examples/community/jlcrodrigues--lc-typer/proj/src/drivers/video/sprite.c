#include "sprite.h"

static int initialized = 0;

static xpm_row_t *letters_xpm[] = {
  a_xpm, b_xpm, c_xpm, d_xpm, e_xpm, f_xpm, g_xpm, h_xpm, i_xpm,
  j_xpm, k_xpm, l_xpm, m_xpm, n_xpm, o_xpm, p_xpm, q_xpm, r_xpm,
  s_xpm, t_xpm, u_xpm, v_xpm, w_xpm, x_xpm, y_xpm, z_xpm};

static xpm_row_t *digits_xpm[] = {zero, one, two, three, four, five, six, seven, eight, nine, percentage, slash, dots};

static Sprite sprite_font[26];
static Sprite sprite_digits[11];

Sprite (create_sprite)(xpm_map_t xpm) {
  Sprite sprite;
  enum xpm_image_type type = XPM_8_8_8;
  sprite.pix_map = xpm_load(xpm, type, &(sprite.img));
  return sprite;
}

void load_font(void) {
  for (int i = 0; i < 26; i++) 
    sprite_font[i] = create_sprite(letters_xpm[i]);
  for (int i = 0; i < 11; i++) 
    sprite_digits[i] = create_sprite(digits_xpm[i]);
  initialized = 1;
}

int draw_char(char letter, int pos_x, int pos_y, uint32_t color) {
  if (!initialized) load_font();
  Sprite sprite;
  if (letter <= '9' && letter >= '0') {
    draw_letter(sprite_digits[(int) (letter - '0')], pos_x, pos_y, color);
    return 0;
  }
  else if (letter <= 'z' && letter >= 'a') {
    draw_letter(sprite_font[(int) (letter - 'a')], pos_x, pos_y, color);
    return 0;
  }
  else if (letter == '%') {
    sprite = create_sprite(digits_xpm[10]);
    return draw_letter(sprite, pos_x, pos_y, color);
  }
  else if (letter == '/') {
    sprite = create_sprite(digits_xpm[11]);
    return draw_letter(sprite, pos_x, pos_y, color);
  }
  else if (letter == ':') {
    sprite = create_sprite(digits_xpm[12]);
    return draw_letter(sprite, pos_x, pos_y, color);
  }
  else 
    return 1;
}

int draw_sentence(char *sentence, int pos_x, int pos_y, uint32_t color) {
  int init_pos_x = pos_x;
  int i = 0;
  while (sentence[i] != '\0') {
    if (sentence[i] == '\n') {
      i++;
      pos_y += FONT_HEIGHT;
      pos_x = init_pos_x;
      continue;
    }
    draw_char(sentence[i], pos_x, pos_y, color);
    pos_x += FONT_WIDTH + PADDING;
    i++;
  }
  return 0;
}

int get_sentence_width(char *sentence) {
  int size = 0, i = 0;
  while (sentence[i] != '\0') {
    size += FONT_WIDTH + PADDING;
    i++;
  }
  return (strlen(sentence) * (PADDING + FONT_WIDTH)) - PADDING;
}
