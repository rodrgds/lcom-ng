#include "explosion_viewer.h"
#include "../../assets/assets.h"
#include "../../engine/graphics/renderer.h"

void explosion_viewer_draw(const Explosion *e) {
  if (e == NULL || !explosion_is_active(e)) return;

  uint8_t frame_count = assets_get_explosion_count();
  if (frame_count == 0) return;

  Sprite *const *frames = assets_get_explosion();
  if (frames == NULL) return;

  uint8_t current_tick = explosion_get_current_tick(e);
  uint8_t total_ticks = explosion_get_total_ticks(e);
  if (total_ticks == 0) return;

  uint8_t frame_index = (current_tick * frame_count) / total_ticks;
  if (frame_index >= frame_count) frame_index = frame_count - 1;

  const Sprite *frame = frames[frame_index];
  if (frame == NULL) return;

  // Center the sprite over the explosion coordinates
  int draw_x = explosion_get_x(e) - (sprite_get_width(frame) / 2);
  int draw_y = explosion_get_y(e) - (sprite_get_height(frame) / 2);

  renderer_draw_sprite(frame, draw_x, draw_y);
}
