#include "sprite.h"

extern uint8_t *img_buffer;
extern uint16_t x_resolution;
extern uint16_t y_resolution;
extern unsigned bytes_per_pixel;

Sprite* letter_sprites[NUM_LETTERS] = {0};
Sprite* number_sprites[NUM_NUMBERS] = {0};

Sprite* gameTitle = NULL;
Sprite* mouseCursor = NULL;

static xpm_map_t letter_xpms[NUM_LETTERS] = {
  (xpm_map_t)a_xpm, (xpm_map_t)b_xpm, (xpm_map_t)c_xpm, (xpm_map_t)d_xpm, (xpm_map_t)e_xpm, (xpm_map_t)f_xpm,
  (xpm_map_t)g_xpm, (xpm_map_t)h_xpm, (xpm_map_t)i_xpm, (xpm_map_t)j_xpm, (xpm_map_t)k_xpm, (xpm_map_t)l_xpm, (xpm_map_t)m_xpm,
  (xpm_map_t)n_xpm, (xpm_map_t)o_xpm, (xpm_map_t)p_xpm, (xpm_map_t)q_xpm, (xpm_map_t)r_xpm, (xpm_map_t)s_xpm, (xpm_map_t)t_xpm,
  (xpm_map_t)u_xpm, (xpm_map_t)v_xpm, (xpm_map_t)w_xpm, (xpm_map_t)x_xpm, (xpm_map_t)y_xpm, (xpm_map_t)z_xpm
};

static xpm_map_t number_xpms[NUM_NUMBERS] = {
  (xpm_map_t)zero_xpm, (xpm_map_t)one_xpm, (xpm_map_t)two_xpm, (xpm_map_t)three_xpm, (xpm_map_t)four_xpm,
  (xpm_map_t)five_xpm, (xpm_map_t)six_xpm, (xpm_map_t)seven_xpm, (xpm_map_t)eight_xpm, (xpm_map_t)nine_xpm
};

Sprite* create_sprite(xpm_map_t xpm_map) {
  Sprite* sp = (Sprite*) malloc(sizeof(Sprite));
  if (sp == NULL) {
    return NULL;
  }

  xpm_image_t img;
  sp->map = (uint32_t*) xpm_load(xpm_map, XPM_8_8_8_8, &img);
  sp->width = img.width;
  sp->height = img.height;

  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  return sp;
}

int draw_sprite(Sprite* sprite, uint16_t x, uint16_t y) {
  uint16_t width = sprite->width;
  uint16_t height = sprite->height;

  for (unsigned i = 0; i < height; i++) {
    for (unsigned j = 0; j < width; j++) {
      uint32_t color = sprite->map[j + i * width];
      uint16_t px = x + j;
      uint16_t py = y + i;
      
      if (color == 0xFFFFFF) continue;
      if (px >= x_resolution || py >= y_resolution) continue;
      memcpy(&img_buffer[(x_resolution * py + px) * bytes_per_pixel], &color, bytes_per_pixel);
    }
  }

  return 0;
}

void destroy_sprite(Sprite* sprite) {
  if (sprite == NULL) {
    return;
  }
  else if (sprite->map != NULL) {
    free(sprite->map);
  }

  free(sprite);
  sprite = NULL;
}

void load_sprites() {
  gameTitle = create_sprite((xpm_map_t) title);
  mouseCursor = create_sprite((xpm_map_t) mouse_cursor_xpm);

  for (int i = 0; i < NUM_LETTERS; ++i)
    letter_sprites[i] = create_sprite(letter_xpms[i]);

  for (int i = 0; i < NUM_NUMBERS; ++i)
    number_sprites[i] = create_sprite(number_xpms[i]);
}

void unload_sprites() {
  destroy_sprite(gameTitle);
  destroy_sprite(mouseCursor);

  for (int i = 0; i < NUM_LETTERS; ++i)
    destroy_sprite(letter_sprites[i]);
  for (int i = 0; i < NUM_NUMBERS; ++i)
    destroy_sprite(number_sprites[i]);
}
