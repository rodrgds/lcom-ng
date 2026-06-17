#include "menu.h"

#include "ninjix_platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtc.h"
#include "video_gr_ext.h"

#include "profiler.h"
#include "resources.h"
#include "scene.h"

#include "colors.h"
#include "panel.h"
#include "scancodes.h"
#include "sprite_utils.h"
#include "text_utils.h"
#include "hud_font.h"

#define MENU_TITLE_SCALE 4
#define MENU_OPTION_SCALE 2
#define MENU_SMALL_SCALE 2
#define INSTRUCTIONS_TEXT_SCALE 2
#define MENU_OPTION_HEIGHT 40
#define MENU_OPTION_GAP 16
#define MENU_OPTION_PADDING (MENU_OPTION_HEIGHT + MENU_OPTION_GAP)

#define MENU_SCREEN_WIDTH 800
#define MENU_SCREEN_HEIGHT 600

#define MENU_TITLE_Y 60
#define MENU_LOGO_Y_OFFSET 12
#define MENU_OPTIONS_START_Y 200
#define MENU_DATETIME_Y 560
#define MENU_DATETIME_X 20

#define MENU_PANEL_WIDTH 500
#define MENU_PANEL_HEIGHT 420
#define MENU_PANEL_Y 60
#define MENU_PANEL_X ((MENU_SCREEN_WIDTH - MENU_PANEL_WIDTH) / 2U)

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
} menu_rect_t;

static const menu_rect_t path_segments[] = {
  {206, 248,  58, 202},
  {206, 248, 178,  58},
  {326, 248,  58, 140},
  {326, 330, 190,  58},
  {458, 154,  58, 234},
  {380, 154, 136,  58},
};

static int menu_draw_grass_tile(const vg_sprite_t *grass_tileset, uint16_t variant_x,
                                uint16_t variant_y, uint16_t dst_x, uint16_t dst_y)
{
  return vg_draw_sprite_region_scaled_to_buffer(grass_tileset, variant_x, variant_y, 16, 16,
                                                dst_x, dst_y, MAP_TILE_SCALE);
}

static int menu_draw_scene_background(const vg_sprite_t *grass_tileset, const vg_sprite_t *shrine)
{
  if (vg_fill_rect_to_buffer(0, 0, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, COLOR_MAP_BORDER) != 0)
    return 1;

  if (vg_fill_rect_to_buffer(8, 8, (uint16_t)(MENU_SCREEN_WIDTH - 16),
                             (uint16_t)(MENU_SCREEN_HEIGHT - 16), COLOR_GRASS_BACKDROP) != 0)
    return 1;

  for (uint16_t y = 8; y + 31 < MENU_SCREEN_HEIGHT; y = (uint16_t)(y + 32))
  {
    for (uint16_t x = 8; x + 31 < MENU_SCREEN_WIDTH; x = (uint16_t)(x + 32))
    {
      uint16_t tile_x = (uint16_t)(x / 32U);
      uint16_t tile_y = (uint16_t)(y / 32U);
      uint32_t hash = (uint32_t)(tile_x * 73856093U) ^ (uint32_t)(tile_y * 19349663U) ^ 0x9E3779B9U;

      // Choose from the 4x2 bottom options in the tilesheet
      uint16_t variant_x = (uint16_t)((hash % 4U) * 16U);
      uint16_t variant_y = (uint16_t)(((hash / 4U) % 2U) == 0U ? 176U : 192U);

      if (menu_draw_grass_tile(grass_tileset, variant_x, variant_y, x, y) != 0)
        return 1;
    }
  }

  for (size_t index = 0; index < sizeof(path_segments) / sizeof(path_segments[0]); index++)
  {
    const menu_rect_t *segment = &path_segments[index];
    if (vg_fill_rect_to_buffer(segment->x, segment->y, segment->width,
                               segment->height, COLOR_PATH_FILL) != 0)
      return 1;
  }

  if (vg_draw_sprite_region_scaled_to_buffer(shrine, 0, 0, SHRINE_FRAME, SHRINE_FRAME,
                                             384, 142, 2) != 0)
    return 1;

  return 0;
}

static int menu_draw_decorative_units(const menu_t *menu)
{
  if (menu == NULL || menu->resources == NULL || !menu->resources->loaded)
    return 0;

  // Left ninja
  if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->tower_basic_sprites[0], 0, 0,
                                            TOWER_FRAME_WIDTH, TOWER_FRAME_HEIGHT,
                                            50, 100, 1) != 0)
    return 1;

  // Center ninja
  if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->tower_smoke_sprites[0], 0, 0,
                                            TOWER_FRAME_WIDTH, TOWER_FRAME_HEIGHT,
                                            400, 120, 1) != 0)
    return 1;

  // Right ninja
  if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->tower_poison_sprites[0], 0, 0,
                                            TOWER_FRAME_WIDTH, TOWER_FRAME_HEIGHT,
                                            700, 110, 1) != 0)
    return 1;

  // Draw enemy units
  if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->enemy_sprites[ENEMY_KIND_NORMAL], 0, 0, 32, 32,
                                            120, 400, 1) != 0)
    return 1;

  if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->enemy_sprites[ENEMY_KIND_FAST], 0, 0, 32, 32,
                                            650, 420, 1) != 0)
    return 1;

  if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->enemy_sprites[ENEMY_KIND_HEALING], 0, 0, 32, 32,
                                            380, 480, 1) != 0)
    return 1;

  return 0;
}

static void menu_update_datetime(menu_t *menu, uint32_t current_tick)
{
  if (current_tick - menu->last_rtc_update < menu->timer_hz)
    return;

  menu->last_rtc_update = current_tick;

  rtc_date rtc_date_val;
  if (rtc_read_date(&rtc_date_val) != 0)
    return;

  menu->datetime.day = rtc_date_val.day;
  menu->datetime.month = rtc_date_val.month;
  menu->datetime.year = rtc_date_val.year;

  rtc_time rtc_time_val;
  if (rtc_read_time(&rtc_time_val) != 0)
    return;

  menu->datetime.hour = rtc_time_val.hour;
  menu->datetime.minute = rtc_time_val.minute;
  menu->datetime.second = rtc_time_val.second;
}

static uint16_t menu_get_option_y(int16_t option_index)
{
  return MENU_OPTIONS_START_Y + option_index * MENU_OPTION_PADDING;
}

static void menu_get_option_bounds(int16_t option_index, uint16_t *left, uint16_t *right,
                                   uint16_t *top, uint16_t *bottom)
{
  uint16_t y = menu_get_option_y(option_index);
  uint16_t height = MENU_OPTION_HEIGHT;
  uint16_t center_x = MENU_SCREEN_WIDTH / 2;
  uint16_t width = 300;

  *left = center_x - width / 2;
  *right = center_x + width / 2;
  *top = y;
  *bottom = y + height;
}

static int16_t menu_get_hovered_option(int16_t cursor_x, int16_t cursor_y)
{
  for (int16_t i = 0; i < MENU_OPTION_COUNT; i++)
  {
    uint16_t left, right, top, bottom;
    menu_get_option_bounds(i, &left, &right, &top, &bottom);

    if (cursor_x >= (int16_t)left && cursor_x <= (int16_t)right && cursor_y >= (int16_t)top &&
        cursor_y <= (int16_t)bottom)
    {
      return i;
    }
  }
  return -1;
}

static const char *menu_get_option_text(menu_option_t option)
{
  switch (option)
  {
    case MENU_OPTION_PLAY_SINGLE:
      return "PLAY - SINGLE";
    case MENU_OPTION_PLAY_MULTI:
      return "PLAY - MULTI";
    case MENU_OPTION_INSTRUCTIONS:
      return "INSTRUCTIONS";
    case MENU_OPTION_CREDITS:
      return "CREDITS";
    case MENU_OPTION_EXIT:
      return "EXIT";
    default:
      return "???";
  }
}

static void menu_destroy_cached_static_screens(menu_t *menu)
{
  if (menu == NULL)
    return;

  vg_destroy_sprite(&menu->background_sprite);
  vg_destroy_sprite(&menu->panel_logo_sprite);
  vg_destroy_sprite(&menu->instructions_sprite);
  vg_destroy_sprite(&menu->credits_sprite);
  menu->background_cached = false;
  menu->panel_logo_cached = false;
  menu->instructions_cached = false;
  menu->credits_cached = false;
}

static int menu_cache_current_buffer(vg_sprite_t *sprite, bool *cached)
{
  if (sprite == NULL || cached == NULL)
    return 1;

  if (sprite->pixels == NULL)
  {
    if (sprite_utils_create(sprite, (uint16_t)get_h_res(), (uint16_t)get_v_res()) != 0)
      return 1;
  }

  if (vg_capture_buffer_to_sprite(sprite) != 0)
    return 1;

  *cached = true;
  return 0;
}

static int menu_draw_background(menu_t *menu)
{
  if (menu->background_cached)
  {
    if (vg_opaque_sprite_to_buffer(&menu->background_sprite, 0, 0) != 0)
      return 1;
    return 0;
  }

  if (menu->resources != NULL && menu->resources->loaded)
  {
    if (menu_draw_scene_background(&menu->resources->grass_tileset_sprite,
                                   &menu->resources->shrine_sprite) != 0)
      return 1;
    (void)menu_cache_current_buffer(&menu->background_sprite, &menu->background_cached);
    return 0;
  }

  return vg_clear_buffer(COLOR_GRASS_BACKDROP);
}

static int menu_cache_panel_region(menu_t *menu, uint16_t panel_x, uint16_t panel_y)
{
  if (menu == NULL)
    return 1;

  if (menu->panel_logo_sprite.pixels == NULL)
  {
    if (sprite_utils_create(&menu->panel_logo_sprite, MENU_PANEL_WIDTH, MENU_PANEL_HEIGHT) != 0)
      return 1;
  }

  if (vg_capture_region_to_sprite(&menu->panel_logo_sprite, panel_x, panel_y) != 0)
    return 1;

  menu->panel_logo_cached = true;
  return 0;
}

static int menu_draw_cached_screen_with_cursor(const menu_t *menu, const vg_sprite_t *screen)
{
  if (vg_opaque_sprite_to_buffer(screen, 0, 0) != 0)
    return 1;

  if (menu->resources != NULL && menu->resources->loaded)
  {
    if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->cursor_sprite, 0, 0,
                                              menu->resources->cursor_sprite.width,
                                              menu->resources->cursor_sprite.height,
                                              menu->cursor_x, menu->cursor_y, 1) != 0)
      return 1;
  }

  return vg_present_buffer();
}

int menu_init(menu_t *menu, const resources_t *resources, uint16_t timer_hz, uint16_t render_hz)
{
  if (menu == NULL || resources == NULL || !resources->loaded)
    return 1;

  memset(menu, 0, sizeof(*menu));

  menu->current_screen = MENU_SCREEN_MAIN;
  menu->selected_option = 0;
  menu->hovered_option = -1;
  menu->cursor_x = MENU_SCREEN_WIDTH / 2;
  menu->cursor_y = MENU_SCREEN_HEIGHT / 2;
  menu->timer_hz = timer_hz;
  menu->render_hz = render_hz;
  menu->last_rtc_update = 0;
  menu->resources = resources;

  rtc_date rtc_date_val;
  if (rtc_read_date(&rtc_date_val) != 0)
  {
    printf("menu_init(): rtc_read_date() failed (non-fatal)\n");
  }
  else
  {
    menu->datetime.day = rtc_date_val.day;
    menu->datetime.month = rtc_date_val.month;
    menu->datetime.year = rtc_date_val.year;
  }

  rtc_time rtc_time_val;
  if (rtc_read_time(&rtc_time_val) != 0)
  {
    printf("menu_init(): rtc_read_time() failed (non-fatal)\n");
  }
  else
  {
    menu->datetime.hour = rtc_time_val.hour;
    menu->datetime.minute = rtc_time_val.minute;
    menu->datetime.second = rtc_time_val.second;
  }

  return 0;
}

void menu_shutdown(menu_t *menu)
{
  if (menu == NULL)
    return;

  menu_destroy_cached_static_screens(menu);

  memset(menu, 0, sizeof(*menu));
}

menu_result_t menu_update(menu_t *menu, uint32_t current_tick)
{
  if (menu == NULL)
    return MENU_RESULT_NONE;

  menu_update_datetime(menu, current_tick);
  menu->frame_count++;

  return MENU_RESULT_NONE;
}


static int menu_draw_tower_shop_card(const menu_t *menu,
                                     const vg_sprite_t *sprite,
                                     uint16_t cx, uint16_t cy,
                                     uint16_t card_w, uint16_t card_h,
                                     const char *name,  const char *price,
                                     const char *dmg,   const char *spd,
                                     const char *rng,   const char *tip)
{

  if (vg_fill_rect_to_buffer(cx, cy, card_w, card_h, COLOR_CARD_BORDER) != 0)
    return 1;
  if (vg_fill_rect_to_buffer((uint16_t)(cx + 2), (uint16_t)(cy + 2),
                              (uint16_t)(card_w - 4), (uint16_t)(card_h - 4),
                              COLOR_CARD_FILL) != 0)
    return 1;

  const uint16_t icon_scale  = 2;
  const uint16_t icon_w      = (uint16_t)(sprite->width  * icon_scale);
  const uint16_t icon_h      = (uint16_t)(sprite->height * icon_scale);
  const uint16_t icon_x      = (uint16_t)(cx + (card_w - icon_w) / 2);
  const uint16_t icon_y      = (uint16_t)(cy + 4);

  if (vg_draw_sprite_region_scaled_to_buffer(sprite, 0, 0,
                                             sprite->width, sprite->height,
                                             (int16_t)icon_x, (int16_t)icon_y,
                                             icon_scale) != 0)
    return 1;

  uint16_t cursor_y = (uint16_t)(icon_y + icon_h + 3);

  uint16_t name_w = text_utils_estimate_width(name, 1);
  int16_t  name_x = (int16_t)(cx + (card_w - name_w) / 2);
  if (hud_font_draw_text_colored(&menu->resources->font, name,
                                 name_x, (int16_t)cursor_y,
                                 1, COLOR_TEXT_DARK) != 0)
    return 1;
  cursor_y = (uint16_t)(cursor_y + 10);

  uint16_t price_w = text_utils_estimate_width(price, 1);
  int16_t  price_x = (int16_t)(cx + (card_w - price_w) / 2);
  
  if (hud_font_draw_text_colored(&menu->resources->font, price,
    price_x + 4, (int16_t)(cursor_y + 3),
    1, COLOR_TEXT_DARK) != 0)
    return 1;
  if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->icon_cash_sprite, 0, 0,
                                             menu->resources->icon_cash_sprite.width, menu->resources->icon_cash_sprite.height,
                                             (int16_t)(price_x - 5), (int16_t)(cursor_y + 1),
                                             1) != 0)
    return 1;
  cursor_y = (uint16_t)(cursor_y + 20);

  char stat_buf[20];
  snprintf(stat_buf, sizeof(stat_buf), "D:%-3s S:%-3s R:%-3s", dmg, spd, rng);
  uint16_t stat_w = text_utils_estimate_width(stat_buf, 1);
  int16_t  stat_x = (int16_t)(cx + (card_w - stat_w) / 2);
  if (hud_font_draw_text_colored(&menu->resources->font, stat_buf,
                                 stat_x, (int16_t)cursor_y,
                                 1, COLOR_TEXT_DIM) != 0)
    return 1;
  cursor_y = (uint16_t)(cursor_y + 10);

  uint16_t tip_w = text_utils_estimate_width(tip, 1);
  int16_t  tip_x = (int16_t)(cx + (card_w - tip_w) / 2);
  if (hud_font_draw_text_colored(&menu->resources->font, tip,
                                 tip_x, (int16_t)cursor_y,
                                 1, COLOR_TEXT_DIM) != 0)
    return 1;

  return 0;
}

static int menu_draw_shop_row(const menu_t *menu,
                              uint16_t row_top,
                              uint16_t panel_x,
                              uint16_t panel_w)
{
  if (menu == NULL || menu->resources == NULL || !menu->resources->loaded)
    return 0;

  const uint16_t card_w     = 128;
  const uint16_t card_h     = 94;
  const uint16_t card_gap   = 8;
  const uint16_t total_w    = (uint16_t)(4 * card_w + 3 * card_gap);
  const uint16_t row_x      = (uint16_t)(panel_x + (panel_w - total_w) / 2);

  typedef struct {
    const vg_sprite_t *sprite;
    const char        *name;
    const char        *price;
    const char        *dmg;
    const char        *spd;
    const char        *rng;
    const char        *tip;
  } card_def_t;

  const card_def_t cards[4] = {
    { &menu->resources->tower_shop_sprites[TOWER_KIND_BASIC][SHOP_SPRITE_NORMAL],  "NORMAL", "90",  "MED", "MED", "MED", "All-round defender" },
    { &menu->resources->tower_shop_sprites[TOWER_KIND_GUN][SHOP_SPRITE_NORMAL],    "SNIPER", "155", "HI",  "LOW", "MAX", "Long-range picks"   },
    { &menu->resources->tower_shop_sprites[TOWER_KIND_SMOKE][SHOP_SPRITE_NORMAL],  "SMOKE",  "125", "LOW", "MED",  "MED", "Slows/reveals"   },
    { &menu->resources->tower_shop_sprites[TOWER_KIND_POISON][SHOP_SPRITE_NORMAL], "POISON", "170", "MED", "MED", "MED", "Poisons enemies" },
  };

  for (int ci = 0; ci < 4; ci++)
  {
    const card_def_t *c  = &cards[ci];
    uint16_t          cx = (uint16_t)(row_x + (uint16_t)ci * (card_w + card_gap));

    if (menu_draw_tower_shop_card(menu, c->sprite, cx, row_top,
                                  card_w, card_h,
                                  c->name, c->price,
                                  c->dmg, c->spd, c->rng,
                                  c->tip) != 0)
      return 1;
  }

  return 0;
}

static int menu_render_instructions(menu_t *menu)
{
  if (menu->instructions_cached)
    return menu_draw_cached_screen_with_cursor(menu, &menu->instructions_sprite);

  if (menu_draw_background(menu) != 0)
    return 1;

  if (menu_draw_decorative_units(menu) != 0)
    return 1;

  const uint16_t panel_width  = 600;
  const uint16_t panel_height = 500;
  const uint16_t panel_x      = (MENU_SCREEN_WIDTH - panel_width) / 2;
  const uint16_t panel_y      = 30;

  if (panel_draw(panel_x, panel_y, panel_width, panel_height) != 0)
    return 1;

  const char *title      = "HOW TO PLAY";
  uint16_t    title_width = text_utils_estimate_width(title, MENU_TITLE_SCALE);
  int16_t     title_x    = (int16_t)(MENU_SCREEN_WIDTH / 2) - (int16_t)(title_width / 2);
  int16_t     title_y    = (int16_t)(panel_y + 18);

  if (hud_font_draw_text_colored(&menu->resources->font, title, title_x, title_y,
                                 MENU_TITLE_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  const uint16_t content_x      = panel_x + 20;
  const uint16_t line_height    = 18;
  const uint16_t section_spacing = 22;
  int16_t        y              = (int16_t)(panel_y + 66);

  /* Objective */
  if (hud_font_draw_text_colored(&menu->resources->font, "OBJECTIVE:",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;
  y = (int16_t)(y + line_height);
  if (hud_font_draw_text_colored(&menu->resources->font, "Stop enemy waves from reaching the shrine",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  y = (int16_t)(y + line_height);
  if (hud_font_draw_text_colored(&menu->resources->font, "by placing ninja defenders.",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  y = (int16_t)(y + section_spacing);

  /* Mouse controls */
  if (hud_font_draw_text_colored(&menu->resources->font, "MOUSE CONTROLS:",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;
  y = (int16_t)(y + line_height);
  if (hud_font_draw_text_colored(&menu->resources->font, "Left Click  - Place tower on map",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  y = (int16_t)(y + line_height);
  if (hud_font_draw_text_colored(&menu->resources->font, "Right Click - Cancel/Deselect",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  y = (int16_t)(y + section_spacing);

  /* Keyboard shortcuts */
  if (hud_font_draw_text_colored(&menu->resources->font, "KEYBOARD SHORTCUTS:",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;
  y = (int16_t)(y + line_height);
  if (hud_font_draw_text_colored(&menu->resources->font, "ESC       - Pause",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  y = (int16_t)(y + line_height);
  if (hud_font_draw_text_colored(&menu->resources->font, "1-4       - Select tower type",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  y = (int16_t)(y + line_height);
  if (hud_font_draw_text_colored(&menu->resources->font, "ENTER     - Confirm",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  y = (int16_t)(y + section_spacing);

  /* Wave info */
  if (hud_font_draw_text_colored(&menu->resources->font, "WAVES START AUTOMATICALLY.",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_GRASS_LIGHT) != 0)
    return 1;
  y = (int16_t)(y + line_height);
  if (hud_font_draw_text_colored(&menu->resources->font, "Each wave gets progressively harder",
                                 (int16_t)content_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;
  y = (int16_t)(y + section_spacing + 40);

  const char *shop_title      = "TOWER SHOP:";
  uint16_t    shop_title_w    = text_utils_estimate_width(shop_title, INSTRUCTIONS_TEXT_SCALE);
  int16_t     shop_title_x    = (int16_t)(MENU_SCREEN_WIDTH / 2) - (int16_t)(shop_title_w / 2);

  if (hud_font_draw_text_colored(&menu->resources->font, shop_title,
                                 shop_title_x, y,
                                 INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;
  y = (int16_t)(y + line_height + 4);

  if (menu_draw_shop_row(menu, (uint16_t)y, panel_x, panel_width) != 0)
    return 1;

  const uint16_t info_panel_height = 50;
  const uint16_t info_panel_y      = MENU_SCREEN_HEIGHT - info_panel_height - 8;

  if (panel_draw(20, info_panel_y, (uint16_t)(MENU_SCREEN_WIDTH - 40),
                      info_panel_height) != 0)
    return 1;

  const char *back_text  = "Press ESC or BACKSPACE to return to menu";
  uint16_t    back_width = text_utils_estimate_width(back_text, MENU_SMALL_SCALE);
  int16_t     back_x     = (int16_t)(MENU_SCREEN_WIDTH / 2) - (int16_t)(back_width / 2);

  if (hud_font_draw_text_colored(&menu->resources->font, back_text,
                                 back_x, (int16_t)(info_panel_y + 15),
                                 MENU_SMALL_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  (void)menu_cache_current_buffer(&menu->instructions_sprite, &menu->instructions_cached);

  /* ── cursor ── */
  if (menu->resources != NULL && menu->resources->loaded)
  {
    if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->cursor_sprite, 0, 0,
                                              menu->resources->cursor_sprite.width,
                                              menu->resources->cursor_sprite.height,
                                              menu->cursor_x, menu->cursor_y, 1) != 0)
      return 1;
  }

  return vg_present_buffer();
}

static int menu_render_credits(menu_t *menu)
{
  if (menu->credits_cached)
    return menu_draw_cached_screen_with_cursor(menu, &menu->credits_sprite);

  if (menu_draw_background(menu) != 0)
    return 1;

  if (menu_draw_decorative_units(menu) != 0)
    return 1;

  /* ── main content panel ── */
  const uint16_t panel_width  = 600;
  const uint16_t panel_height = 400;
  const uint16_t panel_x      = (MENU_SCREEN_WIDTH - panel_width) / 2;
  const uint16_t panel_y      = 80;

  if (panel_draw(panel_x, panel_y, panel_width, panel_height) != 0)
    return 1;

  /* ── "CREDITS" title ── */
  const char *title       = "CREDITS";
  uint16_t    title_width  = text_utils_estimate_width(title, MENU_TITLE_SCALE);
  int16_t     title_x      = (int16_t)(MENU_SCREEN_WIDTH / 2) - (int16_t)(title_width / 2);
  int16_t     title_y      = (int16_t)(panel_y + 18);

  if (hud_font_draw_text_colored(&menu->resources->font, title, title_x, title_y,
                                  MENU_TITLE_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  /* ── section separator line ── */
  const uint16_t sep_x = panel_x + 20;
  const uint16_t sep_y = (uint16_t)(panel_y + 70);
  const uint16_t sep_w = panel_width - 40;
  if (vg_fill_rect_to_buffer(sep_x, sep_y, sep_w, 2, COLOR_PANEL_BORDER_MID) != 0)
    return 1;

  /* ── team members ── */
  const uint16_t content_x   = panel_x + 20;
  const uint16_t line_height = 36;
  int16_t        y           = (int16_t)(sep_y + 20);

  typedef struct { const char *id; const char *name; } member_t;
  static const member_t members[4] = {
    { "up202403579", "Alexandre Teixeira" },
    { "up202404033", "Carlos Diogo"       },
    { "up202107525", "Pedro Marques"      },
    { "up202404130", "Rodrigo Dias"       },
  };

  for (size_t i = 0; i < 4; i++)
  {
    /* ID in dim colour */
    if (hud_font_draw_text_colored(&menu->resources->font, members[i].id,
                                    (int16_t)content_x, y,
                                    INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
      return 1;

    /* separator dash */
    const char *dash   = " - ";
    uint16_t    id_w   = text_utils_estimate_width(members[i].id, INSTRUCTIONS_TEXT_SCALE);
    int16_t     dash_x = (int16_t)(content_x + id_w);
    if (hud_font_draw_text_colored(&menu->resources->font, dash,
                                    dash_x, y,
                                    INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
      return 1;

    /* name in dark colour */
    uint16_t dash_w  = text_utils_estimate_width(dash, INSTRUCTIONS_TEXT_SCALE);
    int16_t  name_x  = (int16_t)(content_x + id_w + dash_w);
    if (hud_font_draw_text_colored(&menu->resources->font, members[i].name,
                                    name_x, y,
                                    INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DARK) != 0)
      return 1;

    y = (int16_t)(y + line_height);
  }

  /* course / institution / year */
  const uint16_t section_spacing = 22;
  y = (int16_t)(y + section_spacing);

  if (hud_font_draw_text_colored(&menu->resources->font, "LCOM  -  FEUP  -  2025/26",
                                  (int16_t)content_x, y,
                                  INSTRUCTIONS_TEXT_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;

  /* ── bottom back-button panel ── */
  const uint16_t info_panel_height = 50;
  const uint16_t info_panel_y      = MENU_SCREEN_HEIGHT - info_panel_height - 8;

  if (panel_draw(20, info_panel_y,
                      (uint16_t)(MENU_SCREEN_WIDTH - 40),
                      info_panel_height) != 0)
    return 1;

  const char *back_text  = "Press ESC or BACKSPACE to return to menu";
  uint16_t    back_width  = text_utils_estimate_width(back_text, MENU_SMALL_SCALE);
  int16_t     back_x      = (int16_t)(MENU_SCREEN_WIDTH / 2) - (int16_t)(back_width / 2);

  if (hud_font_draw_text_colored(&menu->resources->font, back_text,
                                  back_x, (int16_t)(info_panel_y + 15),
                                  MENU_SMALL_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  (void)menu_cache_current_buffer(&menu->credits_sprite, &menu->credits_cached);

  /* ── cursor ── */
  if (menu->resources != NULL && menu->resources->loaded)
  {
    if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->cursor_sprite, 0, 0,
                                               menu->resources->cursor_sprite.width,
                                               menu->resources->cursor_sprite.height,
                                               menu->cursor_x, menu->cursor_y, 1) != 0)
      return 1;
  }

  return vg_present_buffer();
}

static int menu_render_main(menu_t *menu)
{
  if (menu_draw_background(menu) != 0)
    return 1;

  if (menu_draw_decorative_units(menu) != 0)
    return 1;

  uint16_t panel_width = MENU_PANEL_WIDTH;
  uint16_t panel_height = MENU_PANEL_HEIGHT;
  uint16_t panel_x = MENU_PANEL_X;
  uint16_t panel_y = MENU_PANEL_Y;

  if (menu->panel_logo_cached)
  {
    if (vg_opaque_sprite_to_buffer(&menu->panel_logo_sprite, (int16_t)panel_x, (int16_t)panel_y) != 0)
      return 1;
  }
  else
  {
    if (panel_draw(panel_x, panel_y, panel_width, panel_height) != 0)
      return 1;

    if (menu->resources != NULL && menu->resources->loaded)
    {
      const vg_sprite_t *logo = &menu->resources->menu_logo_sprite;
      const int16_t logo_x = (int16_t)((MENU_SCREEN_WIDTH - logo->width) / 2U);
      const int16_t logo_y = (int16_t)(panel_y + MENU_LOGO_Y_OFFSET);

      if (vg_draw_sprite_region_scaled_to_buffer(logo, 0, 0,
                                                 logo->width, logo->height,
                                                 logo_x, logo_y, 1) != 0)
        return 1;
    }

    (void)menu_cache_panel_region(menu, panel_x, panel_y);
  }

  for (int16_t i = 0; i < MENU_OPTION_COUNT; i++)
  {
    const char *option_text = menu_get_option_text((menu_option_t)i);
    uint16_t y = menu_get_option_y(i);

    uint16_t option_width = 380;
    uint16_t option_height = MENU_OPTION_HEIGHT;
    uint16_t option_x = (MENU_SCREEN_WIDTH - option_width) / 2;
    uint16_t option_y = y;

    uint32_t bg_color = COLOR_CARD_FILL;
    uint32_t border_color = COLOR_CARD_BORDER;
    uint32_t text_color = COLOR_TEXT_DARK;

    if (i == menu->hovered_option)
    {
      bg_color = COLOR_GRASS_LIGHT;
      border_color = COLOR_TEXT_DARK;
      text_color = 0xffffff;
    }
    else if (i == menu->selected_option)
    {
      bg_color = COLOR_CARD_BORDER;
      border_color = COLOR_TEXT_DARK;
      text_color = COLOR_TEXT_SOFT;
    }

    // Draw button border (3 pixels)
    if (vg_fill_rect_to_buffer(option_x, option_y, option_width, option_height, border_color) != 0)
      return 1;

    // Draw button background with padding
    if (vg_fill_rect_to_buffer((uint16_t)(option_x + 3), (uint16_t)(option_y + 3),
                               (uint16_t)(option_width - 6), (uint16_t)(option_height - 6),
                               bg_color) != 0)
      return 1;

    uint16_t opt_text_width = 0;
    for (const char *p = option_text; *p != '\0'; p++)
      opt_text_width += (6U * MENU_OPTION_SCALE);
    uint16_t opt_text_height = 12U * MENU_OPTION_SCALE;
    int16_t opt_text_x = (int16_t)(option_x + 3) + (int16_t)((option_width - 6) / 2) - (int16_t)(opt_text_width / 2);
    int16_t opt_text_y = (int16_t)(option_y + 7) + (int16_t)((option_height - 6) / 2) - (int16_t)(opt_text_height / 2);
    if (hud_font_draw_text_colored(&menu->resources->font, option_text, opt_text_x, opt_text_y, MENU_OPTION_SCALE, text_color) != 0)
      return 1;
  }

  uint16_t info_panel_height = 60;
  uint16_t info_panel_y = MENU_SCREEN_HEIGHT - info_panel_height - 8;

  if (panel_draw(20, info_panel_y, (uint16_t)(MENU_SCREEN_WIDTH - 40), info_panel_height) != 0)
    return 1;

  char datetime_str[32];
  snprintf(datetime_str, sizeof(datetime_str), "%02d/%02d/%02d %02d:%02d:%02d", menu->datetime.day,
           menu->datetime.month, menu->datetime.year, menu->datetime.hour, menu->datetime.minute,
           menu->datetime.second);
  hud_font_draw_text_colored(&menu->resources->font, datetime_str, 35, (uint16_t)(info_panel_y + 22),
                             MENU_SMALL_SCALE, COLOR_TEXT_DARK);

  char fps_str[16];
  snprintf(fps_str, sizeof(fps_str), "FPS: %u", (unsigned)menu->render_hz);
  hud_font_draw_text_colored(&menu->resources->font, fps_str, (uint16_t)(MENU_SCREEN_WIDTH - 120),
                             (uint16_t)(info_panel_y + 22), MENU_SMALL_SCALE, COLOR_TEXT_DARK);

  // Draw cursor sprite
  if (menu->resources != NULL && menu->resources->loaded)
  {
    if (vg_draw_sprite_region_scaled_to_buffer(&menu->resources->cursor_sprite, 0, 0,
                                              menu->resources->cursor_sprite.width,
                                              menu->resources->cursor_sprite.height,
                                              menu->cursor_x, menu->cursor_y, 1) != 0)
      return 1;
  }

  return vg_present_buffer();
}

static int menu_render_multiplayer_background(menu_t *menu)
{
  if (menu_draw_background(menu) != 0)
    return 1;

  return menu_draw_decorative_units(menu);
}

static int menu_draw_cursor(const menu_t *menu)
{
  if (menu == NULL || menu->resources == NULL || !menu->resources->loaded)
    return 1;

  return vg_draw_sprite_region_scaled_to_buffer(&menu->resources->cursor_sprite, 0, 0,
                                                menu->resources->cursor_sprite.width,
                                                menu->resources->cursor_sprite.height,
                                                menu->cursor_x, menu->cursor_y, 1);
}

static int menu_draw_centered_text(const menu_t *menu, const char *text,
                                   uint16_t x, uint16_t y, uint16_t width,
                                   uint16_t scale, uint32_t color)
{
  uint16_t text_width = text_utils_estimate_width(text, scale);
  int16_t text_x = (int16_t)(x + width / 2U) - (int16_t)(text_width / 2U);
  return hud_font_draw_text_colored(&menu->resources->font, text,
                                    text_x, (int16_t)y, scale, color);
}

int menu_render_multiplayer_wait(menu_t *menu, const char *status_text)
{
  if (menu == NULL || menu->resources == NULL || !menu->resources->loaded)
    return 1;

  if (status_text == NULL)
    status_text = "WAITING FOR CONNECTION...";

  if (menu_render_multiplayer_background(menu) != 0)
    return 1;

  const uint16_t panel_w = 520;
  const uint16_t panel_h = 230;
  const uint16_t panel_x = (uint16_t)((MENU_SCREEN_WIDTH - panel_w) / 2U);
  const uint16_t panel_y = 155;

  if (panel_draw(panel_x, panel_y, panel_w, panel_h) != 0)
    return 1;

  if (menu_draw_centered_text(menu, "MULTIPLAYER", panel_x, (uint16_t)(panel_y + 38),
                              panel_w, MENU_TITLE_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  if (menu_draw_centered_text(menu, status_text, panel_x, (uint16_t)(panel_y + 112),
                              panel_w, MENU_OPTION_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;

  if (menu_draw_centered_text(menu, "PRESS ESC TO CANCEL", panel_x, (uint16_t)(panel_y + 168),
                              panel_w, MENU_SMALL_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  if (menu_draw_cursor(menu) != 0)
    return 1;

  return vg_present_buffer();
}

static int menu_draw_role_button(const menu_t *menu, const overlay_menu_t *role_menu,
                                 uint8_t option, const char *label, bool chosen)
{
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  overlay_menu_get_option_bounds(role_menu, option, &x, &y, &width, &height);

  bool active = role_menu->hovered_option == option ||
                (role_menu->hovered_option < 0 &&
                 role_menu->selected_option == (int16_t)option);
  uint32_t bg_color = chosen ? COLOR_GRASS_LIGHT :
                      active ? COLOR_CARD_BORDER : COLOR_CARD_FILL;
  uint32_t border_color = active || chosen ? COLOR_TEXT_DARK : COLOR_CARD_BORDER;
  uint32_t text_color = active || chosen ? COLOR_TEXT_SOFT : COLOR_TEXT_DARK;

  if (vg_fill_rect_to_buffer(x, y, width, height, border_color) != 0)
    return 1;
  if (vg_fill_rect_to_buffer((uint16_t)(x + 3), (uint16_t)(y + 3),
                             (uint16_t)(width - 6), (uint16_t)(height - 6),
                             bg_color) != 0)
    return 1;

  return menu_draw_centered_text(menu, label, x, (uint16_t)(y + 12),
                                 width, MENU_OPTION_SCALE, text_color);
}

int menu_render_multiplayer_roles(menu_t *menu, const overlay_menu_t *role_menu,
                                  multiplayer_role_t local_role,
                                  multiplayer_role_t peer_role,
                                  const char *status_text)
{
  if (menu == NULL || role_menu == NULL ||
      menu->resources == NULL || !menu->resources->loaded)
    return 1;

  if (status_text == NULL)
    status_text = "CHOOSE YOUR ROLE";

  if (menu_render_multiplayer_background(menu) != 0)
    return 1;

  const uint16_t panel_w = 560;
  const uint16_t panel_h = 300;
  const uint16_t panel_x = (uint16_t)((MENU_SCREEN_WIDTH - panel_w) / 2U);
  const uint16_t panel_y = 125;

  if (panel_draw(panel_x, panel_y, panel_w, panel_h) != 0)
    return 1;

  if (menu_draw_centered_text(menu, "CHOOSE ROLES", panel_x, (uint16_t)(panel_y + 34),
                              panel_w, MENU_TITLE_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  if (menu_draw_centered_text(menu, status_text, panel_x, (uint16_t)(panel_y + 92),
                              panel_w, MENU_SMALL_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;

  if (menu_draw_role_button(menu, role_menu, 0, "ATTACK",
                            local_role == MULTIPLAYER_ROLE_ATTACKER) != 0)
    return 1;
  if (menu_draw_role_button(menu, role_menu, 1, "DEFEND",
                            local_role == MULTIPLAYER_ROLE_DEFENDER) != 0)
    return 1;

  char role_line[64];
  snprintf(role_line, sizeof(role_line), "YOU: %s    PEER: %s",
           multiplayer_role_name(local_role),
           multiplayer_role_name(peer_role));

  if (menu_draw_centered_text(menu, role_line, panel_x, (uint16_t)(panel_y + 222),
                              panel_w, MENU_SMALL_SCALE, COLOR_TEXT_DARK) != 0)
    return 1;

  if (menu_draw_centered_text(menu, "PRESS ESC TO CANCEL", panel_x, (uint16_t)(panel_y + 252),
                              panel_w, MENU_SMALL_SCALE, COLOR_TEXT_DIM) != 0)
    return 1;

  if (menu_draw_cursor(menu) != 0)
    return 1;

  return vg_present_buffer();
}

int menu_render(menu_t *menu)
{
  if (menu == NULL || menu->resources == NULL || !menu->resources->loaded)
    return 1;

  switch (menu->current_screen)
  {
    case MENU_SCREEN_MAIN:
    {
      PROFILE_BEGIN("menu_main");
      int result = menu_render_main(menu);
      PROFILE_END();
      return result;
    }
    case MENU_SCREEN_INSTRUCTIONS:
    {
      PROFILE_BEGIN("menu_instructions");
      int result = menu_render_instructions(menu);
      PROFILE_END();
      return result;
    }
    case MENU_SCREEN_CREDITS:
    {
      PROFILE_BEGIN("menu_credits");
      int result = menu_render_credits(menu);
      PROFILE_END();
      return result;
    }
    default:
    {
      PROFILE_BEGIN("menu_main");
      int result = menu_render_main(menu);
      PROFILE_END();
      return result;
    }
  }
}

menu_result_t menu_handle_keyboard(menu_t *menu, bool make, uint8_t scancode)
{
  if (menu == NULL)
    return MENU_RESULT_NONE;

  if (menu->current_screen != MENU_SCREEN_MAIN)
  {
    if (make && (scancode == SCANCODE_ESC || scancode == SCANCODE_BACKSPACE))
    {
      menu->current_screen = MENU_SCREEN_MAIN;
      menu->selected_option = 0;
      return MENU_RESULT_BACK;
    }

    return MENU_RESULT_NONE;
  }

  if (scancode == SCANCODE_UP || scancode == SCANCODE_W)
  {
    menu->selected_option--;
    if (menu->selected_option < 0)
      menu->selected_option = MENU_OPTION_COUNT - 1;
    return MENU_RESULT_NONE;
  }

  if (scancode == SCANCODE_DOWN || scancode == SCANCODE_S)
  {
    menu->selected_option++;
    if (menu->selected_option >= MENU_OPTION_COUNT)
      menu->selected_option = 0;
    return MENU_RESULT_NONE;
  }

  if (!make)
    return MENU_RESULT_NONE;

  if (scancode == SCANCODE_ENTER)
  {
    menu_option_t option = (menu_option_t)menu->selected_option;
    switch (option)
    {
      case MENU_OPTION_PLAY_SINGLE:
        return MENU_RESULT_PLAY_SINGLE;
      case MENU_OPTION_PLAY_MULTI:
        return MENU_RESULT_PLAY_MULTI;
      case MENU_OPTION_INSTRUCTIONS:
        menu->current_screen = MENU_SCREEN_INSTRUCTIONS;
        return MENU_RESULT_INSTRUCTIONS;
      case MENU_OPTION_CREDITS:
        menu->current_screen = MENU_SCREEN_CREDITS;
        return MENU_RESULT_CREDITS;
      case MENU_OPTION_EXIT:
        return MENU_RESULT_EXIT;
      default:
        return MENU_RESULT_NONE;
    }
  }

  if (scancode == SCANCODE_ESC)
    return MENU_RESULT_EXIT;

  // Go back from Instructions/Credits screen
  if (scancode == SCANCODE_BACKSPACE)
    return MENU_RESULT_NONE;

  // Number keys 1-5 to select options
  if (scancode >= SCANCODE_1 && scancode <= SCANCODE_5)
  {
    menu->selected_option = (int16_t)(scancode - SCANCODE_1);
    return MENU_RESULT_NONE;
  }

  return MENU_RESULT_NONE;
}

void menu_handle_mouse_movement(menu_t *menu, int16_t dx, int16_t dy)
{
  if (menu == NULL)
    return;

  menu->cursor_x += dx;
  menu->cursor_y -= dy;

  if (menu->cursor_x < 0)
    menu->cursor_x = 0;
  if (menu->cursor_x >= (int16_t)MENU_SCREEN_WIDTH)
    menu->cursor_x = MENU_SCREEN_WIDTH - 1;
  if (menu->cursor_y < 0)
    menu->cursor_y = 0;
  if (menu->cursor_y >= (int16_t)MENU_SCREEN_HEIGHT)
    menu->cursor_y = MENU_SCREEN_HEIGHT - 1;

  if (menu->current_screen == MENU_SCREEN_MAIN)
  {
    menu->hovered_option = menu_get_hovered_option(menu->cursor_x, menu->cursor_y);
    menu->selected_option = -1;  // Clear keyboard selection while using mouse
  }
  else
  {
    menu->hovered_option = -1;
  }
}

menu_result_t menu_handle_mouse_click(menu_t *menu)
{
  if (menu == NULL)
    return MENU_RESULT_NONE;

  if (menu->current_screen != MENU_SCREEN_MAIN)
    return MENU_RESULT_NONE;

  int16_t hovered = menu_get_hovered_option(menu->cursor_x, menu->cursor_y);
  if (hovered < 0)
    return MENU_RESULT_NONE;

  menu->selected_option = hovered;
  menu_option_t option = (menu_option_t)hovered;

  switch (option)
  {
    case MENU_OPTION_PLAY_SINGLE:
      return MENU_RESULT_PLAY_SINGLE;
    case MENU_OPTION_PLAY_MULTI:
      return MENU_RESULT_PLAY_MULTI;
    case MENU_OPTION_INSTRUCTIONS:
      menu->current_screen = MENU_SCREEN_INSTRUCTIONS;
      return MENU_RESULT_INSTRUCTIONS;
    case MENU_OPTION_CREDITS:
      menu->current_screen = MENU_SCREEN_CREDITS;
      return MENU_RESULT_CREDITS;
    case MENU_OPTION_EXIT:
      return MENU_RESULT_EXIT;
    default:
      return MENU_RESULT_NONE;
  }
}
