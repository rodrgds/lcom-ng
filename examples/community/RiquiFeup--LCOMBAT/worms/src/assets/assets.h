#ifndef ASSETS_H
#define ASSETS_H

#include <stdint.h>
#include "../engine/graphics/sprite.h"
#include "../engine/graphics/font.h"


/**
 * @brief Loads all game assets into memory.
 *        Call this ONCE before entering the game loop.
 * @return 0 on success, non-zero on allocation failure.
 */
int assets_init(void);

/**
 * @brief Frees all loaded assets.
 *        Call this ONCE during teardown.
 */
void assets_cleanup(void);

Sprite *const *assets_get_walk_l(void);   
Sprite *const *assets_get_walk_r(void);   
uint8_t        assets_get_walk_count(void);

Sprite *const *assets_get_jump_l(void);
Sprite *const *assets_get_jump_r(void);
uint8_t        assets_get_jump_count(void);

Sprite *const *assets_get_die(void);
uint8_t        assets_get_die_count(void);

Sprite *const *assets_get_idle1_l(void);
Sprite *const *assets_get_idle2_l(void);
Sprite *const *assets_get_idle3_l(void);
Sprite *const *assets_get_idle4_l(void);

Sprite *const *assets_get_idle1_r(void);   
Sprite *const *assets_get_idle2_r(void);  
Sprite *const *assets_get_idle3_r(void);  
Sprite *const *assets_get_idle4_r(void);  
uint8_t        assets_get_idle1_count(void);
uint8_t        assets_get_idle2_count(void);
uint8_t        assets_get_idle3_count(void);
uint8_t        assets_get_idle4_count(void);

const Sprite  *assets_get_bazooka(void);
Sprite *const *assets_get_explosion(void); 
uint8_t        assets_get_explosion_count(void);

Sprite *const *assets_get_missile_angles(void);
uint8_t        assets_get_missile_angles_count(void);

Sprite *const *assets_get_grenade_angles(void);
uint8_t        assets_get_grenade_angles_count(void);

const Sprite  *assets_get_grenade_icon(void);

Sprite *const *assets_get_bazooka_pull_l(void);
Sprite *const *assets_get_bazooka_pull_r(void);
uint8_t        assets_get_bazooka_pull_count(void);

Sprite *const *assets_get_bazooka_retract_l(void);
Sprite *const *assets_get_bazooka_retract_r(void);
uint8_t        assets_get_bazooka_retract_count(void);

Sprite *const *assets_get_grenade_pull_l(void);
Sprite *const *assets_get_grenade_pull_r(void);
uint8_t        assets_get_grenade_pull_count(void);

Sprite *const *assets_get_grenade_retract_l(void);
Sprite *const *assets_get_grenade_retract_r(void);
uint8_t        assets_get_grenade_retract_count(void);

Sprite *const *assets_get_fall_l(void);
Sprite *const *assets_get_fall_r(void);
uint8_t        assets_get_fall_count(void);

const Sprite  *assets_get_background(void);
const Sprite  *assets_get_terrain(void);
const Sprite  *assets_get_terrain_dirt(void);
const Sprite  *assets_get_water(void);
const Sprite  *assets_get_wind_l(void);
const Sprite  *assets_get_wind_r(void);

const Sprite  *assets_get_crosshair(void);
const Sprite  *assets_get_arrow(void);
const Sprite  *assets_get_cursor(void);

const Font    *assets_get_font(void);

/**
 * @brief Returns the splash logo sprite, or NULL if not yet loaded.
 *        Enable by placing logo.xpm in src/assets/xpm/ui/logo/ and
 *        adding -DHAVE_LOGO_XPM to CFLAGS.
 */
const Sprite  *assets_get_logo(void);
const Sprite  *assets_get_logo_menu(void);

#endif /* ASSETS_H */
