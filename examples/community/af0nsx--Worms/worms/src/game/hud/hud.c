#include "hud.h"

#include <stdbool.h>
#include <stddef.h>

#include "../../engine/graphics/renderer.h"
#include "../../engine/graphics/font.h"
#include "../../engine/ui/ui_bar.h"
#include "../../assets/assets.h"
#include "../../config.h"

#define WORM_MAX_HP  WORM_INITIAL_HEALTH
#define TEAM_MAX_HP  (WORMS_PER_TEAM * WORM_INITIAL_HEALTH)
#define HUD_WORM_NAME_MAX_LEN 16

static const char *const s_team_names[MAX_TEAMS] = {
  "RED", "BLU"
};

static const uint32_t s_team_colors[MAX_TEAMS] = {
  COLOR_TEAM_RED, COLOR_TEAM_BLUE
};

static const int s_team_bar_x[MAX_TEAMS] = {
  HUD_TEAM_BAR0_X, HUD_TEAM_BAR1_X
};

static const WeaponType s_hotbar_weapons[HUD_HOTBAR_SLOT_COUNT] = {
  WEAPON_BAZOOKA, WEAPON_GRENADE
};

static char s_worm_names[MAX_WORMS][HUD_WORM_NAME_MAX_LEN];
static bool s_worm_names_ready = false;

static void uint_to_str(uint32_t v, char *buf) {
  if (v == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
  char tmp[12];
  int len = 0;
  while (v > 0) { tmp[len++] = (char)('0' + v % 10); v /= 10; }
  for (int i = 0; i < len; i++) buf[i] = tmp[len - 1 - i];
  buf[len] = '\0';
}

static void append_str(char *dst, size_t dst_size, const char *src) {
  size_t i = 0;

  if (dst == NULL || dst_size == 0 || src == NULL) return;

  while (i < dst_size && dst[i] != '\0') i++;
  if (i >= dst_size) return;

  while (*src != '\0' && i + 1 < dst_size) {
    dst[i++] = *src++;
  }
  dst[i] = '\0';
}

static void init_worm_names(void) {
  if (s_worm_names_ready) return;

  for (int wi = 0; wi < MAX_WORMS; wi++) {
    int team = wi / WORMS_PER_TEAM;
    int worm_number = (wi % WORMS_PER_TEAM) + 1;
    char number[12];
    const char *team_name = s_team_names[team];

    if (team_name == NULL) team_name = "TEAM";
    uint_to_str((uint32_t)worm_number, number);

    s_worm_names[wi][0] = '\0';
    append_str(s_worm_names[wi], HUD_WORM_NAME_MAX_LEN, team_name);
    append_str(s_worm_names[wi], HUD_WORM_NAME_MAX_LEN, " ");
    append_str(s_worm_names[wi], HUD_WORM_NAME_MAX_LEN, number);
  }

  s_worm_names_ready = true;
}

static void draw_timer(const Font *f, uint32_t ticks_left) {
  renderer_draw_rect(HUD_TIMER_X, HUD_TIMER_Y, HUD_TIMER_W, HUD_TIMER_H, COLOR_HUD_BORDER);
  renderer_draw_rect(
    HUD_TIMER_X + 2, HUD_TIMER_Y + 2,
    HUD_TIMER_W - 4, HUD_TIMER_H - 4,
    COLOR_HUD_BG);
  char buf[12];
  uint_to_str(ticks_left / GAME_FPS, buf);
  int text_w = font_measure_string(f, buf);
  font_draw_string(f, buf,
    HUD_TIMER_X + (HUD_TIMER_W - text_w) / 2,
    HUD_TIMER_Y + (HUD_TIMER_H - FONT_GLYPH_HEIGHT) / 2);
}

static void draw_team_bars(const Font *f, const GameContext *ctx) {
  for (int team = 0; team < MAX_TEAMS; team++) {
    uint16_t total_hp = 0;
    for (int i = 0; i < WORMS_PER_TEAM; i++) {
      const Worm *w = game_context_get_worm(ctx, team * WORMS_PER_TEAM + i);
      if (w != NULL) total_hp = (uint16_t)(total_hp + worm_get_health(w));
    }

    UiBar bar = {
      .x = s_team_bar_x[team], .y = HUD_TEAM_BAR_Y,
      .w = HUD_TEAM_BAR_W,     .h = HUD_TEAM_BAR_H,
      .border       = HUD_TEAM_BAR_BORDER,
      .border_color = COLOR_HUD_BORDER,
      .bg_color     = COLOR_HUD_BG,
      .fill_ok      = s_team_colors[team]
    };
    ui_bar_draw(&bar, total_hp, (uint16_t)TEAM_MAX_HP);

    const char *name = s_team_names[team];
    int name_w = font_measure_string(f, name);
    font_draw_string(f, name,
      s_team_bar_x[team] + (HUD_TEAM_BAR_W - name_w) / 2,
      HUD_TEAM_NAME_Y);
  }
}

static void draw_worm_overlays(const Font *f, const GameContext *ctx) {
  for (int wi = 0; wi < MAX_WORMS; wi++) {
    const Worm *w = game_context_get_worm(ctx, wi);
    if (w == NULL || !worm_is_alive(w)) continue;

    int cx  = (int)worm_get_x(w) + WORM_HITBOX_WIDTH / 2;
    int top = (int)worm_get_y(w);
    int team = game_context_get_worm_team(wi);

    UiBar bar = {
      .x = cx - HUD_WORM_BAR_W / 2,
      .y = top - HUD_WORM_BAR_OFFSET_Y,
      .w = HUD_WORM_BAR_W,
      .h = HUD_WORM_BAR_H,
      .border       = HUD_WORM_BAR_BORDER,
      .border_color = COLOR_HUD_BORDER,
      .bg_color     = COLOR_HUD_BG,
      .fill_ok      = s_team_colors[team],
      .fill_low     = COLOR_HEALTH_LOW,
      .low_pct      = HUD_TEAM_BAR_LOW_PCT
    };
    ui_bar_draw(&bar, worm_get_health(w), (uint16_t)WORM_MAX_HP);

    const char *name = s_worm_names[wi];
    int name_w = font_measure_string(f, name);
    font_draw_string(f, name, cx - name_w / 2, top - HUD_NAME_OFFSET_Y);
  }
}

static void draw_hotbar(const GameContext *ctx) {
  const Worm *active = game_context_get_active_worm(ctx);
  WeaponType  weapon = (active != NULL) ? worm_get_weapon(active) : WEAPON_NONE;

  const Sprite *const icons[HUD_HOTBAR_SLOT_COUNT] = {
    assets_get_bazooka(),
    assets_get_grenade_icon()
  };

  for (int i = 0; i < HUD_HOTBAR_SLOT_COUNT; i++) {
    int sx = HUD_HOTBAR_X + i * (HUD_HOTBAR_SLOT_W + HUD_HOTBAR_GAP);
    uint32_t fill = (weapon == s_hotbar_weapons[i]) ? COLOR_GREY : COLOR_HUD_BG;

    renderer_draw_rect(sx, HUD_HOTBAR_Y, HUD_HOTBAR_SLOT_W, HUD_HOTBAR_SLOT_H,
                       COLOR_HUD_BORDER);
    renderer_draw_rect(sx + 2, HUD_HOTBAR_Y + 2,
                       HUD_HOTBAR_SLOT_W - 4, HUD_HOTBAR_SLOT_H - 4, fill);

    if (icons[i] != NULL) {
      int ix = sx + (HUD_HOTBAR_SLOT_W - HUD_HOTBAR_ICON_W) / 2;
      int iy = HUD_HOTBAR_Y + (HUD_HOTBAR_SLOT_H - HUD_HOTBAR_ICON_H) / 2;
      renderer_draw_sprite(icons[i], ix, iy);
    }
  }
}

void hud_draw(const GameContext *ctx) {
  if (ctx == NULL) return;
  const Font *f = assets_get_font();
  if (f == NULL) return;

  init_worm_names();
  draw_timer(f, game_context_ticks_left(ctx));
  draw_team_bars(f, ctx);
  draw_worm_overlays(f, ctx);
  draw_hotbar(ctx);
}

WeaponType hud_get_hotbar_weapon(int slot_index) {
  if (slot_index < 0 || slot_index >= HUD_HOTBAR_SLOT_COUNT) return WEAPON_NONE;
  return s_hotbar_weapons[slot_index];
}
