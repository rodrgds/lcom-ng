#include <stddef.h>

#include <lcom/lcf.h>

#include "config.h"
#include "graphics.h"
#include "assets/assets.h"
#include "game/terrain/terrain.h"
#include "game/game.h"
#include "game/state/state_machine.h"

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/worms/trace.txt");
  lcf_log_output("/home/lcom/labs/worms/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();
  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  vbe_mode_info_t vmi;
  if (vbe_get_mode_info(GAME_VIDEO_MODE, &vmi) == 0) {
    if (vmi.BitsPerPixel == 0) return 1;
  }

  if (map_vram(GAME_VIDEO_MODE) != 0) return 1;
  if (set_graphic_mode(GAME_VIDEO_MODE) != 0) return 1;

  if (assets_init() != 0) {
    video_exit();
    return 1;
  }

  if (terrain_init() != 0) {
    assets_cleanup();
    video_exit();
    return 1;
  }

  GameContext *context = game_context_create();
  if (context == NULL) {
    terrain_cleanup();
    assets_cleanup();
    video_exit();
    return 1;
  }

  state_machine_run(context);

  game_context_destroy(context);

  terrain_cleanup();
  assets_cleanup();

  if (video_exit() != 0) return 1;

  return 0;
}
