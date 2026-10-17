#include "graphics/graphics.h"

#include "xpm/xpm.h"
#include "devices/vbe.h"
#include "devices/mouse.h"
#include "events/events.h"
#include "game/layout.h"
#include "state/state.h"
#include "game/model/cell_background.h"

#include "game/view/game.h"
#include "menu/view/menu.h"

static const char **characters_xpm[] = {
  a_xpm,
  b_xpm,
  c_xpm,
  d_xpm,
  e_xpm,
  f_xpm,
  g_xpm,
  h_xpm,
  i_xpm,
  j_xpm,
  k_xpm,
  l_xpm,
  m_xpm,
  n_xpm,
  o_xpm,
  p_xpm,
  q_xpm,
  r_xpm,
  s_xpm,
  t_xpm,
  u_xpm,
  v_xpm,
  w_xpm,
  x_xpm,
  y_xpm,
  z_xpm,
  zero_xpm,
  one_xpm,
  two_xpm,
  three_xpm,
  four_xpm,
  five_xpm,
  six_xpm,
  seven_xpm,
  eight_xpm,
  nine_xpm,
  dot_xpm,
  twodots_xpm,
  exclamation_xpm,
  space_xpm
};

static const char **colors_xpm[] = {
  green_xpm,
  yellow_xpm,
  gray_xpm,
  empty_xpm
};

static xpm_image_t *characters;
static xpm_image_t *colors;
static xpm_image_t cursor;

int (graphics_init)() {
  size_t num_xpms = sizeof(characters_xpm) / sizeof(*characters_xpm);
  characters = malloc(num_xpms * sizeof(xpm_image_t));
  colors = malloc(num_xpms * sizeof(xpm_image_t));

  for (size_t i = 0; i < sizeof(characters_xpm) / sizeof(*characters_xpm); i++) {
    if (xpm_load(characters_xpm[i], XPM_5_6_5, characters + i) == NULL) {
      printf("graphics_init: failed to load character xpm %d\n", i);
      return GRAPHICS_ELOAD;
    }
  }

  for (size_t i = 0; i < sizeof(colors_xpm) / sizeof(*colors_xpm); i++) {
    if (xpm_load(colors_xpm[i], XPM_5_6_5, colors + i) == NULL) {
      printf("graphics_init: failed to load color xpm %d\n", i);
      return GRAPHICS_ELOAD;
    }
  }

  if (xpm_load(cursor_xpm, XPM_5_6_5, &cursor) == NULL) {
    printf("graphics_init: failed to load cursor xpm\n");
    return GRAPHICS_ELOAD;
  }

  event_on(TIMER_TICK, graphics_draw_screen);

  return OKAY;
}

int (graphics_cleanup)() {
  event_unregister(TIMER_TICK, graphics_draw_screen);

  size_t num_xpms = sizeof(characters_xpm) / sizeof(*characters_xpm);
  size_t num_colors_xpms = sizeof(colors_xpm) / sizeof(*colors_xpm);
  
  for (size_t i = 0; i < num_xpms; i++) {
    free(characters[i].bytes);
  }

  for (size_t i = 0; i < num_colors_xpms; i++) {
    free(colors[i].bytes);
  }

  free(cursor.bytes);
  free(characters);
  free(colors);
  return OKAY;
}

void (graphics_draw_char)(uint16_t x, uint16_t y, char ch) {
  if (ch >= 'a' && ch <= 'z') {
    vbe_draw_xpm(x, y, characters + (ch - 'a'));
  } else if (ch >= '0' && ch <= '9') {
    vbe_draw_xpm(x, y, characters + (ch - '0' + ('z' - 'a') + 1));
  } else if (ch == '.') {
    vbe_draw_xpm(x, y, characters + 36);
  } else if (ch == ':'){
    vbe_draw_xpm(x, y, characters + 37);
  } else if (ch == '!') {
    vbe_draw_xpm(x, y, characters + 38);
  } else if (ch == ' ') {
    vbe_draw_xpm(x, y, characters + 39);
  }
}

void (graphics_draw_square)(uint16_t x, uint16_t y, cell_background background) {
  switch (background) {
    case EMPTY:
      vbe_draw_xpm(x, y, colors + 3);
      break;
    case GRAY:
      vbe_draw_xpm(x, y, colors + 2);
      break;
    case YELLOW:
      vbe_draw_xpm(x, y, colors + 1);
      break;
    case GREEN:
      vbe_draw_xpm(x, y, colors);
    default:
      return;
  }
}

void (graphics_draw_text)(uint16_t x, uint16_t y, char str[]) {
  for (size_t i = 0; i < strlen(str); i++) {
    graphics_draw_char(x + (i * 64), y, str[i]);
  }
}

void (graphics_draw_small_text)(uint16_t x, uint16_t y, char str[]) {
  for (size_t i = 0; i < strlen(str); i++) {
    graphics_draw_char(x + (i * 32), y, str[i]);
  }
}

void (graphics_draw_state)() {
  switch (state_get()) {
    case GAME:
      game_draw();
      return;

    case MENU:
      menu_draw();
      return;

    default:
      return;
  }
}

void (graphics_draw_screen)() {
  vbe_fill(0xffff);

  graphics_draw_state();
  vbe_draw_xpm(mouse_get_x(), mouse_get_y(), &cursor);

  vbe_flush();
}
