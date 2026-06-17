/**
 * @file scene.h
 * @brief Rendering entry points for gameplay, menus, panels, and overlays.
 * @ingroup group_scene
 *
 * Scene functions draw from the current app/play state without mutating
 * gameplay rules. Simulation belongs in play_state, enemy, tower, wave,
 * and projectile modules.
 */

#ifndef PROJ_SCENE_H
#define PROJ_SCENE_H

#include "ninjix_platform.h"

#include <stdbool.h>
#include <stdint.h>

#include "enemy_path.h"
#include "enemy_types.h"
#include "multiplayer.h"
#include "overlay_menu.h"
#include "path_map.h"
#include "pause_menu.h"
#include "play_state.h"
#include "resources.h"
#include "tower_field.h"
#include "wave.h"

typedef struct {
  multiplayer_event_type_t type;
  union {
    struct {
      tower_kind_t kind;
      int16_t x;
      int16_t y;
    } tower;
    struct {
      uint16_t index;
      uint8_t level;
    } tower_upgrade;
    struct {
      uint16_t index;
    } tower_sell;
    struct {
      uint16_t index;
      uint8_t mode;
    } tower_target;
    struct {
      enemy_kind_t kind;
      uint16_t wave;
    } enemy;
  } data;
} scene_multiplayer_action_t;

#define MAP_TILE_SIZE 32
#define MAP_ORIGIN_X 8
#define MAP_ORIGIN_Y 8
#define MAP_WIDTH 608
#define SIDEBAR_WIDTH 192

#define ENEMY_FRAME 32
#define SHRINE_FRAME 32

#define HUD_SCALE 2
#define BODY_SCALE 1
#define UI_BUTTON_SCALE 2
#define SHOP_ICON_SCALE 2
#define MAP_TILE_SCALE 2

typedef struct scene_t {
  vg_sprite_t map_sprite;
  vg_sprite_t empty_context_sprite;
  vg_sprite_t enemy_card_bg_normal;
  vg_sprite_t enemy_card_bg_hover;
  vg_sprite_t enemy_card_bg_active;
  vg_sprite_t wave_panel_static;
  const resources_t *resources;
  play_state_t play;
  multiplayer_role_t multiplayer_role;
  int16_t cursor_x;
  int16_t cursor_y;
  int32_t pending_dx;
  int32_t pending_dy;
  int16_t active_enemy_kind;
  uint16_t display_fps;
  uint16_t physics_hz;
  uint16_t render_hz;
  bool empty_context_cached;
  bool wave_panel_static_cached;
  uint16_t wave_panel_static_level_id;
  bool wave_panel_static_defender;
  bool prev_left_button;
} scene_t;

/**
 * @brief Initialise the scene for a given level.
 * @param scene       Scene struct to initialise.
 * @param resources   Loaded resource handles.
 * @param level_id    Zero-based level index.
 * @param physics_hz  Physics tick rate (ticks per second).
 * @param render_hz   Render frame rate (frames per second).
 * @return 0 on success.
 */
int scene_init(scene_t *scene, const resources_t *resources, uint16_t level_id,
               uint16_t physics_hz, uint16_t render_hz);

/**
 * @brief Release scene resources (map sprite, cached panels).
 * @param scene  Scene to tear down.
 */
void scene_shutdown(scene_t *scene);

/**
 * @brief Accumulate mouse motion deltas from a mouse interrupt.
 * @param scene      Scene to update.
 * @param dx         Horizontal delta from the mouse packet.
 * @param dy         Vertical delta from the mouse packet.
 * @param x_overflow True if the X delta overflowed (8-bit wrap).
 * @param y_overflow True if the Y delta overflowed.
 */
void scene_accumulate_mouse_motion(scene_t *scene, int16_t dx, int16_t dy,
                                   bool x_overflow, bool y_overflow);

/**
 * @brief Accumulate a raw mouse packet (button state + deltas).
 * @param scene   Scene to update.
 * @param packet  Raw mouse packet from themouse driver.
 */
void scene_accumulate_mouse_packet(scene_t *scene, const struct packet *packet);

/**
 * @brief Update cursor position from accumulated mouse deltas.
 * @param scene  Scene whose cursor to update.
 */
void scene_step_cursor(scene_t *scene);

/**
 * @brief Run one physics tick (play_state_step).
 * @param scene  Scene whose simulation to advance.
 */
void scene_step_physics(scene_t *scene);

/**
 * @brief Render the full gameplay frame to the back buffer.
 * @param scene  Scene to render.
 * @return 0 on success.
 */
int scene_render(scene_t *scene);

/**
 * @brief Render the gameplay frame with a pause overlay on top.
 * @param scene  Scene to render.
 * @param pause  Pause menu state.
 * @return 0 on success.
 */
int scene_render_paused(scene_t *scene, const pause_menu_t *pause);

/**
 * @brief Render the gameplay frame with a multiplayer pause overlay and connection status text.
 * @param scene        Scene to render.
 * @param pause        Pause menu state.
 * @param status_text  String describing connection or synchronisation status (e.g., "Waiting for Peer...").
 * @return 0 on success.
 */
int scene_render_paused_multiplayer(scene_t *scene, const pause_menu_t *pause,
                                    const char *status_text);

/**
 * @brief Render the gameplay frame with a result overlay (game over / victory).
 * @param scene         Scene to render.
 * @param title         Overlay title text ("GAME OVER", "VICTORY").
 * @param menu          Overlay menu with selectable buttons.
 * @param button_labels Null-terminated array of button label strings.
 * @param status_text   String describing connection or synchronisation status
 *                      (e.g., "Waiting for Peer..."). May be NULL to omit.
 * @return 0 on success.
 */
int scene_render_result(scene_t *scene, const char *title,
                        const overlay_menu_t *menu,
                        const char *const *button_labels,
                        const char *status_text);

/**
 * @brief Handle a mouse click -- shop purchase, tower selection, placement, etc.
 * @param scene  Scene to process the click on.
 */
void scene_handle_mouse_click(scene_t *scene);

/**
 * @brief Set the local player's network role within the scene context.
 * @param scene Scene to configure.
 * @param role  The network identity assigned to this instance (Host or Client).
 */
void scene_set_multiplayer_role(scene_t *scene, multiplayer_role_t role);

/**
 * @brief Process a local click in a multiplayer context, generating an outbound action payload.
 * @param scene  Scene to evaluate.
 * @param action Pointer to populate with the resultant action type and network payload.
 */
void scene_handle_multiplayer_mouse_click(scene_t *scene,
                                          scene_multiplayer_action_t *action);
/**
 * @brief Construct a tower built by the remote player.
 * @param scene Scene state to modify.
 * @param kind  The type of tower to instantiate.
 * @param x     Horizontal map coordinate.
 * @param y     Vertical map coordinate.
 * @return 0 on successful validation and placement, non-zero otherwise.
 */
int scene_apply_remote_tower(scene_t *scene, tower_kind_t kind, int16_t x, int16_t y);

/**
 * @brief Apply an upgrade requested by the remote player to a specific tower.
 * @param scene Scene state to modify.
 * @param index Array index of the targeted tower in the active playing field.
 * @param level The target level index to elevate the tower to.
 * @return 0 on successful upgrade application, non-zero otherwise.
 */
int scene_apply_remote_tower_upgrade(scene_t *scene, uint16_t index, uint8_t level);

/**
 * @brief Process the sale of a local tower initiated by the remote player.
 * @param scene Scene state to modify.
 * @param index Array index of the tower to dismantle and refund.
 * @return 0 on successful removal, non-zero otherwise.
 */
int scene_apply_remote_tower_sell(scene_t *scene, uint16_t index);

/**
 * @brief Alter the targeting algorithm of a local tower based on a remote command.
 * @param scene Scene state to modify.
 * @param index Array index of the targeted tower.
 * @param mode  The priority targeting mode identifier (e.g., First, Last, Strongest).
 * @return 0 on success, non-zero if targeting mode or index is invalid.
 */
int scene_apply_remote_tower_target(scene_t *scene, uint16_t index, uint8_t mode);

/**
 * @brief Spawn an enemy unit sent or triggered by the remote opponent/co-op player.
 * @param scene Scene state to modify.
 * @param kind  The type of enemy unit to spawn.
 * @param wave  The wave index mapping to the current difficulty tier.
 * @return 0 on success, non-zero if spawn parameters fail bounds checking.
 */
int scene_apply_remote_enemy(scene_t *scene, enemy_kind_t kind, uint16_t wave);

/**
 * @brief Cancel the current placement mode via keyboard (Escape).
 * @param scene  Scene to cancel placement in.
 */
void scene_handle_placement_cancel(scene_t *scene);

/* Shared helper functions used by scene sub-modules */

bool scene_point_in_rect(int16_t px, int16_t py,
  uint16_t rx, uint16_t ry,
  uint16_t rw, uint16_t rh);

int scene_draw_text_colored(scene_t *scene, const char *text,
  int16_t x, int16_t y, uint16_t scale, uint32_t color);

int scene_draw_centered_text_colored(scene_t *scene, const char *text,
  uint16_t left, uint16_t top, uint16_t width,
  uint16_t scale, uint32_t color);

int scene_fill_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
  uint32_t border_color, uint32_t fill_color);

int scene_build_wave_panel_static(scene_t *scene, uint16_t card_x,
  uint16_t wave_y, uint16_t card_width, uint16_t wave_height);

#endif
