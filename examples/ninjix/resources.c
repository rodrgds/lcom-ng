#include "resources.h"

#include "ninjix_platform.h"

#include <string.h>

#include "video_gr_ext.h"

#include "xpm/enemies/enemy_basic_armored.xpm"
#include "xpm/enemies/enemy_basic_cash.xpm"
#include "xpm/enemies/enemy_basic_exploding.xpm"
#include "xpm/enemies/enemy_basic_fast.xpm"
#include "xpm/enemies/enemy_basic_healing.xpm"
#include "xpm/enemies/enemy_basic_invisible.xpm"
#include "xpm/enemies/enemy_basic_normal.xpm"
#include "xpm/enemies/enemy_basic_teleporting.xpm"
#include "xpm/levels/tileset_grass.xpm"
#include "xpm/levels/shrine_spritesheet.xpm"
#include "xpm/ninjas/tower_ninja_basic_0-0.xpm"
#include "xpm/ninjas/tower_ninja_basic_0-1.xpm"
#include "xpm/ninjas/tower_ninja_basic_0-2.xpm"
#include "xpm/ninjas/tower_ninja_basic_0-3.xpm"
#include "xpm/ninjas/tower_ninja_basic_0-4.xpm"
#include "xpm/ninjas/tower_ninja_gun_0-0.xpm"
#include "xpm/ninjas/tower_ninja_gun_0-1.xpm"
#include "xpm/ninjas/tower_ninja_gun_0-2.xpm"
#include "xpm/ninjas/tower_ninja_gun_0-3.xpm"
#include "xpm/ninjas/tower_ninja_gun_0-4.xpm"
#include "xpm/ninjas/tower_ninja_smoke_0-0.xpm"
#include "xpm/ninjas/tower_ninja_smoke_0-1.xpm"
#include "xpm/ninjas/tower_ninja_smoke_0-2.xpm"
#include "xpm/ninjas/tower_ninja_smoke_0-3.xpm"
#include "xpm/ninjas/tower_ninja_smoke_0-4.xpm"
#include "xpm/ninjas/tower_ninja_poison_0-0.xpm"
#include "xpm/ninjas/tower_ninja_poison_0-1.xpm"
#include "xpm/ninjas/tower_ninja_poison_0-2.xpm"
#include "xpm/ninjas/tower_ninja_poison_0-3.xpm"
#include "xpm/ninjas/tower_ninja_poison_0-4.xpm"
#include "xpm/projectiles/blowdart.xpm"
#include "xpm/projectiles/bullet.xpm"
#include "xpm/projectiles/shuriken.xpm"
#include "xpm/projectiles/smokebomb.xpm"
#include "xpm/ui/button_green_normal.xpm"
#include "xpm/ui/button_green_hover.xpm"
#include "xpm/ui/button_silver_normal.xpm"
#include "xpm/ui/button_silver_hover.xpm"
#include "xpm/ui/cursor_silver.xpm"
#include "xpm/ui/cursor_brown.xpm"
#include "xpm/ui/cursor_green.xpm"
#include "xpm/ui/cursor_red.xpm"
#include "xpm/ui/icon_cash.xpm"
#include "xpm/ui/icon_elixir.xpm"
#include "xpm/ui/icon_health.xpm"
#include "xpm/ui/icon_pause_white.xpm"
#include "xpm/ui/icon_skull_full.xpm"
#include "xpm/ui/icon_poison.xpm"
#include "xpm/ui/panel.xpm"
#include "xpm/ui/ninjix_logo.xpm"
#include "xpm/ui/texturebutton_tower_basic_normal.xpm"
#include "xpm/ui/texturebutton_tower_basic_hover.xpm"
#include "xpm/ui/texturebutton_tower_basic_pressed.xpm"
#include "xpm/ui/texturebutton_tower_gun_normal.xpm"
#include "xpm/ui/texturebutton_tower_gun_hover.xpm"
#include "xpm/ui/texturebutton_tower_gun_pressed.xpm"
#include "xpm/ui/texturebutton_tower_poison_normal.xpm"
#include "xpm/ui/texturebutton_tower_poison_hover.xpm"
#include "xpm/ui/texturebutton_tower_poison_pressed.xpm"
#include "xpm/ui/texturebutton_tower_smoke_normal.xpm"
#include "xpm/ui/texturebutton_tower_smoke_hover.xpm"
#include "xpm/ui/texturebutton_tower_smoke_pressed.xpm"

static int resources_load_sprite(vg_sprite_t *sprite, xpm_map_t xpm)
{
  return vg_load_xpm_sprite(sprite, xpm);
}

static const xpm_map_t basic_xpm_table[TOWER_MAX_LEVEL] = {
  (xpm_map_t)tower_ninja_basic_0_0,
  (xpm_map_t)tower_ninja_basic_0_1,
  (xpm_map_t)tower_ninja_basic_0_2,
  (xpm_map_t)tower_ninja_basic_0_3,
  (xpm_map_t)tower_ninja_basic_0_4,
};

static const xpm_map_t gun_xpm_table[TOWER_MAX_LEVEL] = {
  (xpm_map_t)tower_ninja_gun_0_0,
  (xpm_map_t)tower_ninja_gun_0_1,
  (xpm_map_t)tower_ninja_gun_0_2,
  (xpm_map_t)tower_ninja_gun_0_3,
  (xpm_map_t)tower_ninja_gun_0_4,
};

static const xpm_map_t smoke_xpm_table[TOWER_MAX_LEVEL] = {
  (xpm_map_t)tower_ninja_smoke_0_0,
  (xpm_map_t)tower_ninja_smoke_0_1,
  (xpm_map_t)tower_ninja_smoke_0_2,
  (xpm_map_t)tower_ninja_smoke_0_3,
  (xpm_map_t)tower_ninja_smoke_0_4,
};

static const xpm_map_t poison_xpm_table[TOWER_MAX_LEVEL] = {
  (xpm_map_t)tower_ninja_poison_0_0,
  (xpm_map_t)tower_ninja_poison_0_1,
  (xpm_map_t)tower_ninja_poison_0_2,
  (xpm_map_t)tower_ninja_poison_0_3,
  (xpm_map_t)tower_ninja_poison_0_4,
};

static int tower_sprites_load_all(resources_t *resources)
{
  for (uint8_t i = 0; i < TOWER_MAX_LEVEL; i++)
  {
    if (resources_load_sprite(&resources->tower_basic_sprites[i], basic_xpm_table[i]) != 0)
      return 1;
    if (resources_load_sprite(&resources->tower_gun_sprites[i], gun_xpm_table[i]) != 0)
      return 1;
    if (resources_load_sprite(&resources->tower_smoke_sprites[i], smoke_xpm_table[i]) != 0)
      return 1;
    if (resources_load_sprite(&resources->tower_poison_sprites[i], poison_xpm_table[i]) != 0)
      return 1;
  }
  return 0;
}

static const xpm_map_t shop_xpm_table[TOWER_KIND_COUNT][SHOP_SPRITE_STATE_COUNT] = {
  [TOWER_KIND_BASIC]  = { (xpm_map_t)texturebutton_tower_basic_normal,  (xpm_map_t)texturebutton_tower_basic_hover,  (xpm_map_t)texturebutton_tower_basic_pressed },
  [TOWER_KIND_GUN]    = { (xpm_map_t)texturebutton_tower_gun_normal,    (xpm_map_t)texturebutton_tower_gun_hover,    (xpm_map_t)texturebutton_tower_gun_pressed },
  [TOWER_KIND_SMOKE]  = { (xpm_map_t)texturebutton_tower_smoke_normal,  (xpm_map_t)texturebutton_tower_smoke_hover,  (xpm_map_t)texturebutton_tower_smoke_pressed },
  [TOWER_KIND_POISON] = { (xpm_map_t)texturebutton_tower_poison_normal, (xpm_map_t)texturebutton_tower_poison_hover, (xpm_map_t)texturebutton_tower_poison_pressed },
};

static int resources_load_tower_shop_sprites(resources_t *resources)
{
  for (uint8_t kind = 0; kind < TOWER_KIND_COUNT; kind++)
  {
    for (uint8_t state = 0; state < SHOP_SPRITE_STATE_COUNT; state++)
    {
      if (resources_load_sprite(&resources->tower_shop_sprites[kind][state], shop_xpm_table[kind][state]) != 0)
        return 1;
    }
  }
  return 0;
}

int resources_load(resources_t *resources)
{
  if (resources == NULL)
    return 1;

  memset(resources, 0, sizeof(*resources));

  if (resources_load_sprite(&resources->cursor_sprite, (xpm_map_t)cursor_silver) != 0 ||
      resources_load_sprite(&resources->cursor_grab_sprite, (xpm_map_t)cursor_brown) != 0 ||
      resources_load_sprite(&resources->cursor_green_sprite, (xpm_map_t)cursor_green) != 0 ||
      resources_load_sprite(&resources->cursor_red_sprite, (xpm_map_t)cursor_red) != 0 ||
      resources_load_sprite(&resources->shrine_sprite, (xpm_map_t)shrine_spritesheet) != 0 ||
      resources_load_sprite(&resources->grass_tileset_sprite, (xpm_map_t)tileset_grass) != 0 ||
      resources_load_sprite(&resources->panel_sprite, (xpm_map_t)panel) != 0 ||
      resources_load_sprite(&resources->menu_logo_sprite, (xpm_map_t)ninjix_logo) != 0 ||
      resources_load_tower_shop_sprites(resources) != 0 ||
      resources_load_sprite(&resources->button_green_sprite, (xpm_map_t)button_green_normal) != 0 ||
      resources_load_sprite(&resources->button_green_hover_sprite, (xpm_map_t)button_green_hover) != 0 ||
      resources_load_sprite(&resources->button_silver_sprite, (xpm_map_t)button_silver_normal) != 0 ||
      resources_load_sprite(&resources->button_silver_hover_sprite, (xpm_map_t)button_silver_hover) != 0 ||
      resources_load_sprite(&resources->icon_cash_sprite, (xpm_map_t)icon_cash) != 0 ||
      resources_load_sprite(&resources->icon_elixir_sprite, (xpm_map_t)icon_elixir) != 0 ||
      resources_load_sprite(&resources->icon_health_sprite, (xpm_map_t)icon_health) != 0 ||
      resources_load_sprite(&resources->icon_pause_sprite, (xpm_map_t)icon_pause_white) != 0 ||
      resources_load_sprite(&resources->icon_skull_full_sprite, (xpm_map_t)icon_skull_full) != 0 ||
      resources_load_sprite(&resources->icon_poison_sprite, (xpm_map_t)icon_poison) != 0 ||
      tower_sprites_load_all(resources) != 0)
  {
    resources_destroy(resources);
    return 1;
  }

  static const xpm_map_t enemy_xpm_table[ENEMY_KIND_COUNT] = {
    [ENEMY_KIND_NORMAL]      = (xpm_map_t)enemy_basic_normal,
    [ENEMY_KIND_FAST]        = (xpm_map_t)enemy_basic_fast,
    [ENEMY_KIND_HEALING]     = (xpm_map_t)enemy_basic_healing,
    [ENEMY_KIND_ARMORED]     = (xpm_map_t)enemy_basic_armored,
    [ENEMY_KIND_EXPLODING]   = (xpm_map_t)enemy_basic_exploding,
    [ENEMY_KIND_CASH]        = (xpm_map_t)enemy_basic_cash,
    [ENEMY_KIND_TELEPORTING] = (xpm_map_t)enemy_basic_teleporting,
    [ENEMY_KIND_INVISIBLE]   = (xpm_map_t)enemy_basic_invisible,
  };

  for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
  {
    if (resources_load_sprite(&resources->enemy_sprites[i], enemy_xpm_table[i]) != 0)
    {
      resources_destroy(resources);
      return 1;
    }
  }

  static const xpm_map_t projectile_xpm_table[PROJECTILE_KIND_COUNT] = {
    [PROJECTILE_SHURIKEN] = (xpm_map_t)shuriken,
    [PROJECTILE_BULLET] = (xpm_map_t)bullet,
    [PROJECTILE_SMOKE_BOMB] = (xpm_map_t)smokebomb,
    [PROJECTILE_POISON_DART] = (xpm_map_t)blowdart,
  };

  for (uint8_t i = 0; i < PROJECTILE_KIND_COUNT; i++)
  {
    if (resources_load_sprite(&resources->projectile_sprites[i], projectile_xpm_table[i]) != 0)
    {
      resources_destroy(resources);
      return 1;
    }
  }

  if (hud_font_load(&resources->font) != 0)
  {
    resources_destroy(resources);
    return 1;
  }

  const vg_sprite_t *enemy_sprite_ptrs[ENEMY_KIND_COUNT];
  for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
    enemy_sprite_ptrs[i] = &resources->enemy_sprites[i];

  enemy_types_init(resources->enemy_types, enemy_sprite_ptrs);

  resources->loaded = true;
  return 0;
}

void resources_destroy(resources_t *resources)
{
  if (resources == NULL)
    return;

  hud_font_unload(&resources->font);
  vg_destroy_sprite(&resources->cursor_sprite);
  vg_destroy_sprite(&resources->cursor_grab_sprite);
  vg_destroy_sprite(&resources->cursor_green_sprite);
  vg_destroy_sprite(&resources->cursor_red_sprite);
  vg_destroy_sprite(&resources->shrine_sprite);
  vg_destroy_sprite(&resources->grass_tileset_sprite);
  vg_destroy_sprite(&resources->panel_sprite);
  vg_destroy_sprite(&resources->menu_logo_sprite);
  for (uint8_t kind = 0; kind < TOWER_KIND_COUNT; kind++)
    for (uint8_t state = 0; state < SHOP_SPRITE_STATE_COUNT; state++)
      vg_destroy_sprite(&resources->tower_shop_sprites[kind][state]);
  vg_destroy_sprite(&resources->button_green_sprite);
  vg_destroy_sprite(&resources->button_green_hover_sprite);
  vg_destroy_sprite(&resources->button_silver_sprite);
  vg_destroy_sprite(&resources->button_silver_hover_sprite);
  vg_destroy_sprite(&resources->icon_cash_sprite);
  vg_destroy_sprite(&resources->icon_elixir_sprite);
  vg_destroy_sprite(&resources->icon_health_sprite);
  vg_destroy_sprite(&resources->icon_pause_sprite);
  vg_destroy_sprite(&resources->icon_skull_full_sprite);
  vg_destroy_sprite(&resources->icon_poison_sprite);
  for (uint8_t i = 0; i < TOWER_MAX_LEVEL; i++)
  {
    vg_destroy_sprite(&resources->tower_basic_sprites[i]);
    vg_destroy_sprite(&resources->tower_gun_sprites[i]);
    vg_destroy_sprite(&resources->tower_smoke_sprites[i]);
    vg_destroy_sprite(&resources->tower_poison_sprites[i]);
  }

  for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
    vg_destroy_sprite(&resources->enemy_sprites[i]);
  for (uint8_t i = 0; i < PROJECTILE_KIND_COUNT; i++)
    vg_destroy_sprite(&resources->projectile_sprites[i]);

  memset(resources, 0, sizeof(*resources));
}
