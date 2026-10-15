#include "banner.h"

int banner_init(banner_t *banner) {
  if (banner == NULL)
    return 1;

  banner->current_timeout = 0;
  banner->timeout = 0;
  banner->msg = 0;

  return 0;
}

int banner_set_message(banner_t *banner, char *msg, uint32_t timeout) {
  if (banner == NULL || msg == NULL)
    return 1;

  char *message = malloc(sizeof(char) * strlen(msg));
  strcpy(message, msg);
	
  banner->msg = message;
  banner->timeout = timeout;
  banner->current_timeout = 0;

  vg_set_redraw();

  return 0;
}

int banner_update_timeout(banner_t *banner) {
  if (banner == NULL || banner->msg == NULL)
    return 1;

  banner->current_timeout++;

  if (banner->current_timeout == banner->timeout) {
    vg_set_redraw();
    free(banner->msg);
    banner->msg = NULL;
  }

  return 0;
}

int banner_draw(banner_t *banner, font_t *font) {
  if (banner == NULL || banner->msg == NULL || font == NULL)
    return 1;

  uint32_t msg_size = 26 * strlen(banner->msg);

  vg_draw_rectangle(vg_get_width() / 2 - 26 - (msg_size / 2), 0,
                    52 + msg_size, 40, 0x9d6025);
  font_print_str(font, banner->msg, vg_get_width() / 2 - (msg_size / 2), 4, 0xffffff);

  return 0;
}
